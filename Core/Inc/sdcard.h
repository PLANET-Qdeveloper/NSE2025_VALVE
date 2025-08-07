/*
 * sdcard.h
 *
 *  Created on: Jul 2, 2020
 *      Author: meh
 */

#include "main.h"
#include "types.h"

bool sd_save_data(SensorData_t *data_buffer);
bool sd_check_status(void);
void SD_init_valve_file_number(void);
