/*
 * fatfs_sd.c
 *
 *  Created on: Jul 2, 2020
 *      Author: meh
 */

#include "stm32f4xx_hal.h"

#include "diskio.h"
#include "fatfs_sd.h"

extern SPI_HandleTypeDef hspi1;
extern volatile uint16_t Timer1, Timer2; /* 1ms Timer Counter for SD card operations */

static volatile DSTATUS Stat = STA_NOINIT; /* ディスク状態フラグ */
static uint8_t CardType;                   /* SDタイプ 0:MMC, 1:SDC, 2:Block addressing */
static uint8_t PowerFlag = 0;              /* 電源状態フラグ */

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

  /* 500ms カウンター準備 */
  Timer2 = 50;
  SPI_RxByte();

  do
  {
    /* 0xFF 値が受信されるまで SPI 通信 */
    res = SPI_RxByte();
  } while ((res != 0xFF) && Timer2);

  return res;
}

/* 電源投入 */
static void SD_PowerOn(void)
{
  uint8_t cmd_arg[6];
  uint32_t Count = 0x1FFF;

  /* Deselect 状態で SPI メッセージを送信して待機状態にする */
  DESELECT();

  for (int i = 0; i < 10; i++)
  {
    SPI_TxByte(0xFF);
  }

  /* SPI チップセレクト */
  SELECT();
  /* 初期 GO_IDLE_STATE 状態遷移 */
  cmd_arg[0] = (CMD0 | 0x40);
  cmd_arg[1] = 0;
  cmd_arg[2] = 0;
  cmd_arg[3] = 0;
  cmd_arg[4] = 0;
  cmd_arg[5] = 0x95;

  /* コマンド送信 */
  for (int i = 0; i < 6; i++)
  {
    SPI_TxByte(cmd_arg[i]);
  }

  /* 応答待機 */
  while ((SPI_RxByte() != 0x01) && Count)
  {
    Count--;
  }

  DESELECT();
  SPI_TxByte(0XFF);

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

  /* 100ms タイマー */
  Timer1 = 10;

  /* 応答待機 */
  do
  {
    token = SPI_RxByte();
  } while ((token == 0xFF) && Timer1);

  /* 0xFE 以外 Token 受信時 エラー処理 */
  if (token != 0xFE)
    return FALSE;

  /* バッファにデータ受信 */
  do
  {
    SPI_RxBytePtr(buff++);
    SPI_RxBytePtr(buff++);
  } while (btr -= 2);
  SPI_RxByte(); /* CRC 無視 */
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
  if (SD_ReadyWait() != 0xFF)
    return FALSE;

  /* トークン送信 */
  SPI_TxByte(token);

  /* データトークンの場合 */
  if (token != 0xFD)
  {
    wc = 0;

    /* 512 バイト データ送信 */
    do
    {
      SPI_TxByte(*buff++);
      SPI_TxByte(*buff++);
    } while (--wc);

    SPI_RxByte(); /* CRC 無視 */
    SPI_RxByte();

    /* データ応答受信 */
    while (i <= 64)
    {
      resp = SPI_RxByte();

      /* エラー応答処理 */
      if ((resp & 0x1F) == 0x05)
        break;

      i++;
    }

    /* SPI 受信バッファ Clear */
    while (SPI_RxByte() == 0)
      ;
  }

  if ((resp & 0x1F) == 0x05)
    return TRUE;
  else
    return FALSE;
}
#endif /* _READONLY */

/* CMD パケット送信 */
static BYTE SD_SendCmd(BYTE cmd, DWORD arg)
{
  uint8_t crc, res;

  /* SDカード待機 */ if (SD_ReadyWait() != 0xFF)
    return 0xFF;

  /* コマンドパケット送信 */
  SPI_TxByte(cmd);               /* Command */
  SPI_TxByte((BYTE)(arg >> 24)); /* Argument[31..24] */
  SPI_TxByte((BYTE)(arg >> 16)); /* Argument[23..16] */
  SPI_TxByte((BYTE)(arg >> 8));  /* Argument[15..8] */
  SPI_TxByte((BYTE)arg);         /* Argument[7..0] */

  /* コマンド別 CRC 準備 */
  crc = 0;
  if (cmd == CMD0)
    crc = 0x95; /* CRC for CMD0(0) */
  if (cmd == CMD8)
    crc = 0x87; /* CRC for CMD8(0x1AA) */

  /* CRC 送信 */
  SPI_TxByte(crc);

  /* CMD12 Stop Reading コマンドの場合は応答バイト一つを捨てる */
  if (cmd == CMD12)
    SPI_RxByte();

  /* 10回以内に正常データを受信する */
  uint8_t n = 10;
  do
  {
    res = SPI_RxByte();
  } while ((res & 0x80) && --n);

  return res;
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

  /* Idle 状態移行 */ if (SD_SendCmd(CMD0, 0) == 1)
  {
    /* タイマー 1秒設定 */
    Timer1 = 100;

    /* SD インターフェース動作条件確認 */
    if (SD_SendCmd(CMD8, 0x1AA) == 1)
    {
      /* SDC Ver2+ */
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

          type = (ocr[0] & 0x40) ? 6 : 2;
        }
      }
    }
    else
    {
      /* SDC Ver1 or MMC */
      type = (SD_SendCmd(CMD55, 0) <= 1 && SD_SendCmd(CMD41, 0) <= 1) ? 2 : 1; /* SDC : MMC */

      do
      {
        if (type == 2)
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
      if (!Timer1 || SD_SendCmd(CMD16, 512) != 0)
      {
        /* ブロック長選択 */
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

  if (!(CardType & 4))
    sector *= 512; /* 指定 sector を Byte addressing 単位に変更 */

  SELECT();
  if (count == 1)
  {
    /* シングルブロック読み取り */
    if ((SD_SendCmd(CMD17, sector) == 0) && SD_RxDataBlock(buff, 512))
      count = 0;
  }
  else
  {
    /* 複数ブロック読み取り */
    if (SD_SendCmd(CMD18, sector) == 0)
    {
      do
      {
        if (!SD_RxDataBlock(buff, 512))
          break;

        buff += 512;
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
  if (!(CardType & 4))
    sector *= 512; /* 指定 sector を Byte addressing 単位に変更 */

  SELECT();

  if (count == 1)
  {
    /* シングルブロック書き込み */
    if ((SD_SendCmd(CMD24, sector) == 0) && SD_TxDataBlock(buff, 0xFE))
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
        if (!SD_TxDataBlock(buff, 0xFC))
          break;

        buff += 512;
      } while (--count);

      if (!SD_TxDataBlock(0, 0xFD))
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
      *(WORD *)buff = 512;
      res = RES_OK;
      break;

    case CTRL_SYNC:
      /* 書き込み同期 */
      if (SD_ReadyWait() == 0xFF)
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
