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

bool sd_save_data(SensorData_t *data_buffer);
bool sd_check_status(void);
void SD_init_valve_file_number(void);
