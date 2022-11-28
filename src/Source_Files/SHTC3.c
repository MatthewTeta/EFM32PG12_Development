
#include "SHTC3.h"
#include <stddef.h>

// Private static
static bool         shtc3_module_open = false;
static I2C_TypeDef *shtc3_i2c_x       = NULL;
static uint8_t      shtc3_rx_buff[SHTC3_RX_MEASURE_BUFF_LEN];

void shtc3_i2c_open(I2C_TypeDef *i2c_x) {
  shtc3_i2c_x = i2c_x;

  // Wait for peripheral to be ready after power on
  timer_delay(SHTC3_POW_MS);

  uint32_t SDA_LOC;
  uint32_t SCL_LOC;
  if (i2c_x == I2C0) {
    SDA_LOC = I2C_ROUTELOC0_SDALOC_LOC15;
    SCL_LOC = I2C_ROUTELOC0_SCLLOC_LOC15;
  } else if (i2c_x == I2C1) {
    SDA_LOC = I2C_ROUTELOC0_SDALOC_LOC6;
    SCL_LOC = I2C_ROUTELOC0_SCLLOC_LOC6;
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
  shtc3_module_open = true;
}

void shtc3_i2c_begin_read(scheduler_event_t cb_event) {
  EFM_ASSERT(shtc3_module_open);
  EFM_ASSERT(shtc3_i2c_x != NULL);
  uint8_t cmd_wakeup[] = SHTC3_WAKEUP_CMD;
  uint8_t cmd_measure[] = SHTC3_MEASURE_CMD;
  uint8_t cmd_sleep[] = SHTC3_SLEEP_CMD;
  i2c_write(shtc3_i2c_x, SHTC3_ADDR, cmd_wakeup, SHTC_CMD_LEN, 0);
  // Wait for peripheral to be ready after wakeup command
  timer_delay(SHTC3_POW_MS);
  i2c_write(shtc3_i2c_x, SHTC3_ADDR, cmd_measure, SHTC_CMD_LEN, 0);
  i2c_read(shtc3_i2c_x, SHTC3_ADDR, shtc3_rx_buff, SHTC3_RX_MEASURE_BUFF_LEN,
           cb_event);
  i2c_write(shtc3_i2c_x, SHTC3_ADDR, cmd_sleep, SHTC_CMD_LEN, 0);
}

bool shtc3_get_temperature_and_humidity(float *temperature, float *humidity) {
  EFM_ASSERT(shtc3_module_open);
  // Parse packet buffer and compute CRC
  uint16_t temp_raw = (shtc3_rx_buff[0] << 8) | shtc3_rx_buff[1];
  uint16_t hum_raw  = (shtc3_rx_buff[3] << 8) | shtc3_rx_buff[4];
  //   uint8_t  temp_crc = shtc3_rx_buff[2];
  //   uint8_t  hum_crc  = shtc3_rx_buff[5];
  // Convert values and return them
  *temperature = (float)(-45 + 175 * temp_raw / (0x01 << 16));
  *humidity    = (float)(100 * hum_raw / (0x01 << 16));
  // TODO: Check CRC values
  return true;
}
