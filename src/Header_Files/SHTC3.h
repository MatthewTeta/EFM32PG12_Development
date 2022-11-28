/*
 * SHTC3.h
 *
 *  Created on: Nov 10, 2022
 *      Author: Matthew Teta
 */
#include "i2c.h"

#ifndef SRC_HEADER_FILES_SHTC3_H_
#define SRC_HEADER_FILES_SHTC3_H_

#define SHTC3_ADDR 0x70
#define SHTC3_MEASURE_CMD                                                      \
  { 0x44, 0xDE }
#define SHTC3_WAKEUP_CMD                                                       \
  { 0x35, 0x17 }
#define SHTC3_SLEEP_CMD                                                        \
  { 0xB0, 0x98 }
#define SHTC_CMD_LEN              2
#define SHTC3_RX_MEASURE_BUFF_LEN 6
#define SHTC3_POW_MS              1
// #define SHTC3_POW_MS              0.25

void shtc3_i2c_open(I2C_TypeDef *i2c_x);
void shtc3_i2c_begin_read(scheduler_event_t event);

/**
 * @brief Pass by reference most recent temperature and humidity values as float
 *
 * @param temperature return value
 * @param humidity return value
 * @return true values are valid
 * @return false values are invalid
 */
bool shtc3_get_temperature_and_humidity(float *temperature, float *humidity);

#endif /* SRC_HEADER_FILES_SHTC3_H_ */
