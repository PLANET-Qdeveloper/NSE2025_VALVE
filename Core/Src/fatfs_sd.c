/*
 * fatfs_sd.c
 *
 *  Created on: Jul 2, 2020
 *      Author: meh
 */

#include "stm32f4xx_hal.h"

#include "diskio.h"
#include "fatfs_sd.h"
#include <stdbool.h>

extern SPI_HandleTypeDef hspi1;
extern volatile uint16_t Timer1, Timer2; /* 1ms Timer Counter for SD card operations */

static volatile DSTATUS Stat = STA_NOINIT;  /* ディスク状態フラグ */
static uint8_t CardType;                    /* SDタイプ 0:MMC, 1:SDC, 2:Block addressing */
static uint8_t PowerFlag = 0;               /* 電源状態フラグ */
static uint32_t next_valve_file_number = 1; /* 次に使用するVALVEファイル番号 */

/* SPI チップセレクト */
static void SELECT(void)
{
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);
}

/* SPI チップデセレクト */
static void DESELECT(void)
{
  HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
}

/* SPI データ送信 */
static void SPI_TxByte(BYTE data)
{
  while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
    ;
  HAL_SPI_Transmit(&hspi1, &data, 1, SPI_TIMEOUT);
}

/* SPI データ送受信 リターン型関数 */
static uint8_t SPI_RxByte(void)
{
  uint8_t dummy, data;
  dummy = 0xFF;
  data = 0;

  while ((HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY))
    ;
  HAL_SPI_TransmitReceive(&hspi1, &dummy, &data, 1, SPI_TIMEOUT);

  return data;
}

/* SPI データ送受信 ポインタ型関数 */
static void SPI_RxBytePtr(uint8_t *buff)
{
  *buff = SPI_RxByte();
}

/* SDカード Ready 待機 */
static uint8_t SD_ReadyWait(void)
{
  uint8_t res;

  /* タイムアウト設定 */
  Timer2 = SD_READY_TIMEOUT_MS / 10; // 500ms / 10ms = 50
  SPI_RxByte();

  do
  {
    /* Ready状態まで待機 */
    res = SPI_RxByte();
  } while ((res != SD_TOKEN_DUMMY) && Timer2);

  return res;
}

/* 電源投入とSDカード初期化 */
static void SD_PowerOn(void)
{
  uint8_t cmd_arg[6];
  uint32_t Count = 0x1FFF;

  /* Deselect 状態で SPI メッセージを送信して待機状態にする */
  DESELECT();

  /* 初期化用のクロックを送信 */
  for (int i = 0; i < SD_INIT_CLOCK_CYCLES; i++)
  {
    SPI_TxByte(SD_TOKEN_DUMMY);
  }

  /* SPI チップセレクト */
  SELECT();

  /* 初期 GO_IDLE_STATE 状態遷移 */
  cmd_arg[0] = CMD0;
  cmd_arg[1] = 0;
  cmd_arg[2] = 0;
  cmd_arg[3] = 0;
  cmd_arg[4] = 0;
  cmd_arg[5] = SD_CRC_CMD0;

  /* コマンド送信 */
  for (int i = 0; i < 6; i++)
  {
    SPI_TxByte(cmd_arg[i]);
  }

  /* 応答待機 */
  while ((SPI_RxByte() != SD_RESPONSE_IDLE) && Count)
  {
    Count--;
  }

  DESELECT();
  SPI_TxByte(SD_TOKEN_DUMMY);

  PowerFlag = 1;
}

/* 電源切断 */
static void SD_PowerOff(void)
{
  PowerFlag = 0;
}

/* 電源状態確認 */
static uint8_t SD_CheckPower(void)
{
  /* 0=off, 1=on */
  return PowerFlag;
}

/* データパケット受信 */
static bool SD_RxDataBlock(BYTE *buff, UINT btr)
{
  uint8_t token;

  /* データ受信タイムアウト設定 */
  Timer1 = SD_DATA_TIMEOUT_MS / 10; // 100ms / 10ms = 10

  /* スタートトークン待機 */
  do
  {
    token = SPI_RxByte();
  } while ((token == SD_TOKEN_DUMMY) && Timer1);

  /* スタートトークン確認 */
  if (token != SD_TOKEN_START_BLOCK)
    return FALSE;

  /* バッファにデータ受信 */
  do
  {
    SPI_RxBytePtr(buff++);
    SPI_RxBytePtr(buff++);
  } while (btr -= 2);

  /* CRC読み捨て（使用しない） */
  SPI_RxByte();
  SPI_RxByte();

  return TRUE;
}

/* データ送信パケット */
#if _READONLY == 0
static bool SD_TxDataBlock(const BYTE *buff, BYTE token)
{
  uint8_t resp, wc;
  uint8_t i = 0;

  /* SDカード準備待機 */
  if (SD_ReadyWait() != SD_TOKEN_DUMMY)
    return FALSE;

  /* トークン送信 */
  SPI_TxByte(token);

  /* データトークンの場合 */
  if (token != SD_TOKEN_STOP_MULTI_WRITE)
  {
    wc = 0;

    /* SDブロックサイズ データ送信 */
    do
    {
      SPI_TxByte(*buff++);
      SPI_TxByte(*buff++);
    } while (--wc);

    /* CRC送信（ダミー） */
    SPI_RxByte();
    SPI_RxByte();

    /* データ応答受信 */
    while (i <= SD_MAX_RETRY_COUNT)
    {
      resp = SPI_RxByte();

      /* データ受信承認確認 */
      if ((resp & SD_DATA_RESPONSE_MASK) == SD_DATA_RESPONSE_ACCEPTED)
        break;

      i++;
    }

    /* 書き込み完了待機 */
    while (SPI_RxByte() == 0)
      ;
  }

  /* データ応答確認 */
  return ((resp & SD_DATA_RESPONSE_MASK) == SD_DATA_RESPONSE_ACCEPTED) ? TRUE : FALSE;
}
#endif /* _READONLY */

/* CMD パケット送信 */
static BYTE SD_SendCmd(BYTE cmd, DWORD arg)
{
  uint8_t crc, res;

  /* SDカード待機状態確認 */
  if (SD_ReadyWait() != SD_TOKEN_DUMMY)
    return SD_RESPONSE_ERROR;

  /* コマンドパケット送信 */
  SPI_TxByte(cmd);               /* Command */
  SPI_TxByte((BYTE)(arg >> 24)); /* Argument[31..24] */
  SPI_TxByte((BYTE)(arg >> 16)); /* Argument[23..16] */
  SPI_TxByte((BYTE)(arg >> 8));  /* Argument[15..8] */
  SPI_TxByte((BYTE)arg);         /* Argument[7..0] */

  /* コマンド別 CRC 準備 */
  crc = 0;
  if (cmd == CMD0)
    crc = SD_CRC_CMD0; /* CRC for CMD0(0) */
  if (cmd == CMD8)
    crc = SD_CRC_CMD8; /* CRC for CMD8(0x1AA) */

  /* CRC 送信 */
  SPI_TxByte(crc);

  /* CMD12 Stop Reading コマンドの場合は応答バイト一つを捨てる */
  if (cmd == CMD12)
    SPI_RxByte();

  /* 指定回数以内に正常応答を受信 */
  uint8_t n = SD_CMD_RETRY_COUNT;
  do
  {
    res = SPI_RxByte();
  } while ((res & 0x80) && --n);

  return res;
}

/* SDカード タイプ判定（Ver2+用）
 * 注意: この関数は現在使用されていませんが、コードの理解とテスト用に保持
 * メインの初期化処理は SD_disk_initialize() 内で直接実装されている
 */
__attribute__((unused)) static uint8_t SD_DetectCardTypeV2(void)
{
  uint8_t n, ocr[4];
  uint8_t type = 0;

  /* OCR情報取得 */
  for (n = 0; n < 4; n++)
  {
    ocr[n] = SPI_RxByte();
  }

  if (ocr[2] == 0x01 && ocr[3] == 0xAA)
  {
    /* 2.7-3.6V 電圧範囲動作 */
    do
    {
      if (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 1UL << 30) == 0)
        break; /* ACMD41 with HCS bit */
    } while (Timer1);

    if (Timer1 && SD_SendCmd(CMD58, 0) == 0)
    {
      /* CCSビットチェック */
      for (n = 0; n < 4; n++)
      {
        ocr[n] = SPI_RxByte();
      }
      type = (ocr[0] & 0x40) ? SD_CARD_TYPE_SDC_V2 : SD_CARD_TYPE_SDC_V1;
    }
  }

  return type;
}

/* SDカード タイプ判定（Ver1/MMC用）
 * 注意: この関数は現在使用されていませんが、コードの理解とテスト用に保持
 * メインの初期化処理は SD_disk_initialize() 内で直接実装されている
 */
__attribute__((unused)) static uint8_t SD_DetectCardTypeV1(void)
{
  uint8_t type;

  /* SDC Ver1 or MMC判定 */
  type = (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 0) <= 1) ? SD_CARD_TYPE_SDC_V1 : SD_CARD_TYPE_MMC;

  do
  {
    if (type == SD_CARD_TYPE_SDC_V1)
    {
      if (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 0) == 0)
        break; /* ACMD41 */
    }
    else
    {
      if (SD_SendCmd(CMD1, 0) == 0)
        break; /* CMD1 */
    }
  } while (Timer1);

  if (!Timer1 || SD_SendCmd(CMD16, SD_BLOCK_SIZE) != 0)
  {
    /* ブロック長選択失敗 */
    type = 0;
  }

  return type;
}

/*-----------------------------------------------------------------------
  fatfsで使用されるGlobal関数群
  user_diskio.c ファイルで使用される
-----------------------------------------------------------------------*/

/* SDカード初期化 */
DSTATUS SD_disk_initialize(BYTE drv)
{
  uint8_t n, type, ocr[4];

  /* 一種類のドライブのみサポート */
  if (drv)
    return STA_NOINIT;

  /* SDカード未挿入 */
  if (Stat & STA_NODISK)
    return Stat;

  /* SDカード Power On */
  SD_PowerOn();

  /* SPI 通信のため Chip Select */
  SELECT();

  /* SDカードタイプ変数初期化 */
  type = 0;

  /* Idle 状態移行確認 */
  if (SD_SendCmd(CMD0, 0) == SD_RESPONSE_IDLE)
  {
    /* 初期化タイムアウト設定 */
    Timer1 = SD_INIT_TIMEOUT_MS / 10; // 1000ms / 10ms = 100

    /* SD インターフェース動作条件確認 */
    if (SD_SendCmd(CMD8, SD_VOLTAGE_RANGE_2V7_3V6) == SD_RESPONSE_IDLE)
    {
      /* SDC Ver2+ 処理 */
      for (n = 0; n < 4; n++)
      {
        ocr[n] = SPI_RxByte();
      }
      if (ocr[2] == 0x01 && ocr[3] == 0xAA)
      {
        /* 2.7-3.6V 電圧範囲動作 */
        do
        {
          if (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 1UL << 30) == 0)
            break; /* ACMD41 with HCS bit */
        } while (Timer1);

        if (Timer1 && SD_SendCmd(CMD58, 0) == 0)
        {
          /* CCSビットチェック */
          for (n = 0; n < 4; n++)
          {
            ocr[n] = SPI_RxByte();
          }
          type = (ocr[0] & 0x40) ? SD_CARD_TYPE_SDC_V2 : SD_CARD_TYPE_SDC_V1;
        }
      }
    }
    else
    {
      /* SDC Ver1 or MMC 処理 */
      type = (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 0) <= 1) ? SD_CARD_TYPE_SDC_V1 : SD_CARD_TYPE_MMC;

      do
      {
        if (type == SD_CARD_TYPE_SDC_V1)
        {
          if (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 0) == 0)
            break; /* ACMD41 */
        }
        else
        {
          if (SD_SendCmd(CMD1, 0) == 0)
            break; /* CMD1 */
        }
      } while (Timer1);

      if (!Timer1 || SD_SendCmd(CMD16, SD_BLOCK_SIZE) != 0)
      {
        /* ブロック長選択失敗 */
        type = 0;
      }
    }
  }

  CardType = type;
  DESELECT();

  SPI_RxByte(); /* Idle 状態遷移 (Release DO) */

  if (type)
  {
    /* STA_NOINITクリア */
    Stat &= ~STA_NOINIT;
  }
  else
  {
    /* 初期化失敗 */
    SD_PowerOff();
  }

  return Stat;
}

/* ディスク状態確認 */
DSTATUS SD_disk_status(BYTE drv)
{
  if (drv)
    return STA_NOINIT;

  return Stat;
}

/* セクタ読み取り */
DRESULT SD_disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
  if (pdrv || !count)
    return RES_PARERR;

  if (Stat & STA_NOINIT)
    return RES_NOTRDY;

  /* バイトアドレッシング変換（必要に応じて） */
  if (!(CardType & 4))
    sector *= SD_BLOCK_SIZE;

  SELECT();
  if (count == 1)
  {
    /* シングルブロック読み取り */
    if ((SD_SendCmd(CMD17, sector) == SD_RESPONSE_SUCCESS) && SD_RxDataBlock(buff, SD_BLOCK_SIZE))
      count = 0;
  }
  else
  {
    /* 複数ブロック読み取り */
    if (SD_SendCmd(CMD18, sector) == 0)
    {
      do
      {
        if (!SD_RxDataBlock(buff, SD_BLOCK_SIZE))
          break;

        buff += SD_BLOCK_SIZE;
      } while (--count);

      /* STOP_TRANSMISSION, 全ブロックを読み取った後、送信停止要求 */
      SD_SendCmd(CMD12, 0);
    }
  }

  DESELECT();
  SPI_RxByte(); /* Idle 状態(Release DO) */

  return count ? RES_ERROR : RES_OK;
}

/* セクタ書き込み */
#if _READONLY == 0
DRESULT SD_disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
  if (pdrv || !count)
    return RES_PARERR;

  if (Stat & STA_NOINIT)
    return RES_NOTRDY;

  if (Stat & STA_PROTECT)
    return RES_WRPRT;
  /* バイトアドレッシング変換（必要に応じて） */
  if (!(CardType & 4))
    sector *= SD_BLOCK_SIZE;

  SELECT();

  if (count == 1)
  {
    /* シングルブロック書き込み */
    if ((SD_SendCmd(CMD24, sector) == SD_RESPONSE_SUCCESS) && SD_TxDataBlock(buff, SD_TOKEN_START_BLOCK))
      count = 0;
  }
  else
  {
    /* 複数ブロック書き込み */
    if (CardType & 2)
    {
      SD_SendCmd(CMD55, 0);
      SD_SendCmd(CMD23, count); /* ACMD23 */
    }

    if (SD_SendCmd(CMD25, sector) == 0)
    {
      do
      {
        if (!SD_TxDataBlock(buff, SD_TOKEN_START_MULTI_WRITE))
          break;

        buff += SD_BLOCK_SIZE;
      } while (--count);

      /* マルチブロック書き込み終了 */
      if (!SD_TxDataBlock(0, SD_TOKEN_STOP_MULTI_WRITE))
      {
        count = 1;
      }
    }
  }

  DESELECT();
  SPI_RxByte();

  return count ? RES_ERROR : RES_OK;
}
#endif /* _READONLY */

/* その他関数 */
DRESULT SD_disk_ioctl(BYTE drv, BYTE ctrl, void *buff)
{
  DRESULT res;
  BYTE n, csd[16], *ptr = buff;
  WORD csize;

  if (drv)
    return RES_PARERR;

  res = RES_ERROR;

  if (ctrl == CTRL_POWER)
  {
    switch (*ptr)
    {
    case 0:
      if (SD_CheckPower())
        SD_PowerOff(); /* 電源オフ */
      res = RES_OK;
      break;
    case 1:
      SD_PowerOn(); /* 電源オン */
      res = RES_OK;
      break;
    case 2:
      *(ptr + 1) = (BYTE)SD_CheckPower();
      res = RES_OK; /* 電源チェック */
      break;
    default:
      res = RES_PARERR;
    }
  }
  else
  {
    if (Stat & STA_NOINIT)
      return RES_NOTRDY;

    SELECT();

    switch (ctrl)
    {
    case GET_SECTOR_COUNT:
      /* SDカード内 Sectorの個数 (DWORD) */
      if ((SD_SendCmd(CMD9, 0) == 0) && SD_RxDataBlock(csd, 16))
      {
        if ((csd[0] >> 6) == 1)
        {
          /* SDC ver 2.00 */
          csize = csd[9] + ((WORD)csd[8] << 8) + 1;
          *(DWORD *)buff = (DWORD)csize << 10;
        }
        else
        {
          /* MMC or SDC ver 1.XX */
          n = (csd[5] & 15) + ((csd[10] & 128) >> 7) + ((csd[9] & 3) << 1) + 2;
          csize = (csd[8] >> 6) + ((WORD)csd[7] << 2) + ((WORD)(csd[6] & 3) << 10) + 1;
          *(DWORD *)buff = (DWORD)csize << (n - 9);
        }

        res = RES_OK;
      }
      break;

    case GET_SECTOR_SIZE:
      /* セクタの単位サイズ (WORD) */
      *(WORD *)buff = SD_BLOCK_SIZE;
      res = RES_OK;
      break;

    case CTRL_SYNC:
      /* 書き込み同期 */
      if (SD_ReadyWait() == SD_TOKEN_DUMMY)
        res = RES_OK;
      break;

    case MMC_GET_CSD:
      /* CSD 情報受信 (16 bytes) */
      if (SD_SendCmd(CMD9, 0) == 0 && SD_RxDataBlock(ptr, 16))
        res = RES_OK;
      break;

    case MMC_GET_CID:
      /* CID 情報受信 (16 bytes) */
      if (SD_SendCmd(CMD10, 0) == 0 && SD_RxDataBlock(ptr, 16))
        res = RES_OK;
      break;
    case MMC_GET_OCR:
      /* OCR 情報受信 (4 bytes) */
      if (SD_SendCmd(CMD58, 0) == 0)
      {
        for (n = 0; n < 4; n++)
        {
          *ptr++ = SPI_RxByte();
        }

        res = RES_OK;
      }
      break;

    default:
      res = RES_PARERR;
    }

    DESELECT();
    SPI_RxByte();
  }

  return res;
}

/* Vファイル番号を初期化（起動時に一度だけ実行） */
void SD_init_valve_file_number(void)
{
  DIR dir;
  FILINFO fno;
  FRESULT res;
  uint32_t max_num = 0;
  uint32_t current_num;

  /* ルートディレクトリを開く */
  res = f_opendir(&dir, "");
  if (res == FR_OK)
  {
    /* 既存のV_x.csvファイルを探して最大番号を取得 */
    for (;;)
    {
      res = f_readdir(&dir, &fno);
      if (res != FR_OK || fno.fname[0] == 0)
        break; /* ディレクトリの終端またはエラー */

      /* V_x.csvファイルかチェック */
      if ((fno.fattrib & AM_DIR) == 0) /* ディレクトリではない */
      {
        char *name = fno.fname;
        /* ファイル名がV_で始まり、.csvで終わるかチェック */
        if (strncmp(name, "V_", 2) == 0)
        {
          char *dot = strrchr(name, '.');
          if (dot != NULL && strcmp(dot, ".csv") == 0)
          {
            /* 番号部分を抽出 */
            char *num_start = name + 2; /* "V_"の後 */
            char *num_end = dot;
            char num_str[16];

            /* 番号部分をコピー */
            size_t num_len = num_end - num_start;
            if (num_len > 0 && num_len < sizeof(num_str))
            {
              strncpy(num_str, num_start, num_len);
              num_str[num_len] = '\0';

              /* 数値に変換 */
              current_num = (uint32_t)atoi(num_str);
              if (current_num > max_num)
              {
                max_num = current_num;
              }
            }
          }
        }
      }
    }
    f_closedir(&dir);
  }

  /* 次の番号を設定 */
  next_valve_file_number = max_num + 1;

  printf("Vファイル番号初期化完了: 次のファイル番号=%lu\r\n", (unsigned long)next_valve_file_number);
}

/* Vファイル名を連番で生成（8.3形式対応） */
void SD_get_valve_filename(char *filename, size_t max_len)
{
  /* パラメータ検証 */
  if (filename == NULL || max_len < 13)
  {
    if (filename != NULL && max_len > 0)
    {
      filename[0] = '\0'; /* エラー時は空文字列 */
    }
    return;
  }

  /* 8.3形式に適合するファイル名を生成 */
  /* 形式: V_x.csv（xは1から始まる連番） */
  snprintf(filename, max_len, "V_%lu.csv", (unsigned long)next_valve_file_number);

  /* 次回のために番号をインクリメント */
  next_valve_file_number++;
}
