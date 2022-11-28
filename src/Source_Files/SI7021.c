/*
 * SI7021.c
 *
 *  Created on: Oct 27, 2022
 *      Author: Matthew Teta
 */
#include "SI7021.h"
#include <stddef.h>

// Private static
static bool         si7021_module_open = false;
static I2C_TypeDef *si7021_i2c_x       = NULL;
static uint8_t      humidity_buff[SI7021_RX_MEASURE_BUFF_LEN];
static uint8_t      temperature_buff[SI7021_RX_MEASURE_BUFF_LEN];

/**
 * @brief Initialize the I2C peripheral with proper clock settings for use with
 * SI7021 Temperature & Humidity sensor
 *
 * @param i2c_x Choose the I2C bus which the sensor is connected to
 */
void si7021_i2c_open(I2C_TypeDef *i2c_x) {
  si7021_i2c_x = i2c_x;

  // Wait for peripheral to be ready after power on
  timer_delay(SI7021_POW_MS);

  uint32_t SDA_LOC;
  uint32_t SCL_LOC;
  if (i2c_x == I2C0) {
    SDA_LOC = I2C_ROUTELOC0_SDALOC_LOC15;
    SCL_LOC = I2C_ROUTELOC0_SCLLOC_LOC15;
  } else if (i2c_x == I2C1) {
    SDA_LOC = I2C_ROUTELOC0_SDALOC_LOC19;
    SCL_LOC = I2C_ROUTELOC0_SCLLOC_LOC19;
  } else
    EFM_ASSERT(false);

  I2C_open_type i2c_open_t = {
      .init =
          {
              .enable  = true,
              .master  = true,
              .clhr    = i2cClockHLRAsymetric,
              .freq    = I2C_FREQ_FAST_MAX,
              .refFreq = 0,
          },
      .SDA_LOC = SDA_LOC,
      .SCL_LOC = SCL_LOC,
      .SCL_EN  = true,
      .SDA_EN  = true,
  };

  i2c_open(i2c_x, &i2c_open_t);
  si7021_module_open = true;
}

void si7021_i2c_begin_read_humidity(uint32_t EVENT_ID) {
  EFM_ASSERT(si7021_module_open);
  EFM_ASSERT(si7021_i2c_x != NULL);
  static uint8_t cmd[] = {SI7021_MEASURE_HUMIDITY_CMD};
  i2c_write(si7021_i2c_x, SI7021_ADDR, cmd, 1, 0);
  i2c_read(si7021_i2c_x, SI7021_ADDR, humidity_buff, SI7021_RX_MEASURE_BUFF_LEN,
           EVENT_ID);
}

void si7021_i2c_begin_read_temperature(uint32_t EVENT_ID) {
  EFM_ASSERT(si7021_module_open);
  EFM_ASSERT(si7021_i2c_x != NULL);
  static uint8_t cmd[] = {SI7021_MEASURE_TEMPERATURE_CMD};
  i2c_write(si7021_i2c_x, SI7021_ADDR, cmd, 1, 0);
  i2c_read(si7021_i2c_x, SI7021_ADDR, temperature_buff,
           SI7021_RX_MEASURE_BUFF_LEN, EVENT_ID);
}

float si7021_get_humidity(void) {
  // Swap byte order
  uint16_t measure_buff =
      (uint16_t)((humidity_buff[0] << 8) | humidity_buff[1]);
  // According to datasheet
  return (125.0 * (float)measure_buff) / (float)(65536) - 6.0;
}

float si7021_get_temperature(void) {
  // Swap byte order
  uint16_t measure_buff =
      (uint16_t)((temperature_buff[0] << 8) | temperature_buff[1]);
  // According to datasheet
  return (175.72 * (float)measure_buff) / (float)(65536) - 46.85;
}
