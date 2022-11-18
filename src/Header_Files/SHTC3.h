/*
 * SHTC3.h
 *
 *  Created on: Nov 10, 2022
 *      Author: Matthew Teta
 */
#include "i2c.h"

#ifndef SRC_HEADER_FILES_SHTC3_H_
#define SRC_HEADER_FILES_SHTC3_H_

#define SHTC3_I2C_TypeDef         I2C1
#define SHTC3_ADDR                0x70
#define SHTC3_MEASURE_CMD         0x44DE
#define SHTC3_WAKEUP_CMD          0x3517
#define SHTC3_SLEEP_CMD           0xB098
#define SHTC3_RX_MEASURE_BUFF_LEN 2
#define SHTC3_POW_MS              0.24

void shtc3_i2c_open(shtc3_open_t s);
void shtc3_i2c_begin_read(scheduler_event_t event);

float shtc3_get_temperature(void);
float shtc3_get_humidity(void);

#endif /* SRC_HEADER_FILES_SHTC3_H_ */
