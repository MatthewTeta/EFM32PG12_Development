/*
 * SI7021.h
 *
 *  Created on: Oct 27, 2022
 *      Author: Matthew Teta
 */
#include "em_i2c.h"

#include "HW_delay.h"
#include "i2c.h"

#ifndef SRC_HEADER_FILES_SI7021_H_
#define SRC_HEADER_FILES_SI7021_H_

#define SI7021_ADDR                    0x40
#define SI7021_MEASURE_HUMIDITY_CMD    0xE5
#define SI7021_MEASURE_TEMPERATURE_CMD 0xE3
#define SI7021_RX_MEASURE_BUFF_LEN     2
#define SI7021_POW_MS                  80

// Configure the given I2C peripheral for use with the SI7021
void si7021_i2c_open(I2C_TypeDef *i2c_x);

// Request a new humidity value with given callback event
void si7021_i2c_begin_read_humidity(uint32_t EVENT_ID);

// Request a new temperature value with given callback event
void si7021_i2c_begin_read_temperature(uint32_t EVENT_ID);

// Returns the most recent humidity measurement
float si7021_get_humidity(void);

// Returns the most recent temperature measurement
float si7021_get_temperature(void);

#endif /* SRC_HEADER_FILES_SI7021_H_ */
