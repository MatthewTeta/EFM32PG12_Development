/*
 * i2c.h
 *
 *  Created on: Oct 16, 2022
 *      Author: Matthew Teta
 */

#include "em_cmu.h"
#include "em_i2c.h"

#include "scheduler.h"
#include "sleep_routines.h"

#ifndef SRC_HEADER_FILES_I2C_H_
#define SRC_HEADER_FILES_I2C_H_

#define I2C_EM_BLOCK EM1
#define I2C_R        (0x01)
#define I2C_W        (0x00)

#define I2C_QUEUE_CAPACITY 128

typedef struct {
  I2C_Init_TypeDef init;
  uint32_t         SDA_LOC;
  uint32_t         SCL_LOC;
  bool             SDA_EN;
  bool             SCL_EN;
} I2C_open_type;

/**
 * Function Prototypes
 */

/**
 * Reset the I2C Bus and set it up the peripheral for master mode
 * @param * i2c_x
 * @param i2c_open Instance of struct with all of the necessary input arguments
 * (all are required)
 */
void i2c_open(I2C_TypeDef *i2c_x, I2C_open_type *i2c_open);

// void i2c_start(I2C_TypeDef *i2c_x, uint32_t recieve, uint8_t addr_dev,
// uint8_t cmd_dev, uint8_t *buff, uint32_t buff_len, uint32_t EVENT_ID);

/**
 * @brief Begin an i2c read job, blocks until any previous job on the same bus
 * is finished
 *
 * @param i2c_x which bus to read on
 * @param device_addr device address (7 bits not shifted)
 * @param buff pointer to uint8_t buffer for which bytes will be stores into
 * @param n_bytes number of bytes to read
 * @param cb_event scheduler callback event (use 0 for no cb event)
 */
void i2c_read(I2C_TypeDef *i2c_x, uint8_t device_addr, uint8_t buff[],
              uint32_t n_bytes, scheduler_event_t cb_event);

/**
 * @brief Begin an i2c write job, blocks until any previous job is finished on
 * the same i2c bus
 *
 * @param i2c_x which bus to write to
 * @param device_addr device address (7 bits not shifted)
 * @param buff buffer to transmit
 * @param n_bytes length of transmit buffer
 * @param cb_event scheduler callback event (use 0 for no cb event)
 */
void i2c_write(I2C_TypeDef *i2c_x, uint8_t device_addr, uint8_t buff[],
               uint32_t n_bytes, scheduler_event_t cb_event);

// void i2c_stop(I2C_TypeDef *i2c_x);

// IRQ Handlers
void I2C0_IRQHandler(void);
void I2C1_IRQHandler(void);

#endif /* SRC_HEADER_FILES_I2C_H_ */
