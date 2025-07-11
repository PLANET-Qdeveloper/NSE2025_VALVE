/*
 * sdcard.h
 *
 *  Created on: Jul 2, 2020
 *      Author: meh
 */

#include "diskio.h"
#include "main.h"
#include "types.h"
#include <stddef.h>

bool sd_save_data(const SensorData_t *data_buffer, uint32_t data_count);
bool sd_check_status(void);
void SD_init_valve_file_number(void);
void SD_get_filename(char *filename, size_t max_len);
