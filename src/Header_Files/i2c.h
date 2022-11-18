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
  uint32_t         SDALOC;
  uint32_t         SCLLOC;
  bool             SDA_EN;
  bool             SCL_EN;
} I2C_open_type;

/**
 * Function Prototypes
 */

/**
 * Reset the I2C Bus and set it up the peripheral for master mode
 * @param * i2c_x
 * @param i2c_open
 */
void i2c_open(I2C_TypeDef *i2c_x, I2C_open_type *i2c_open);

// void i2c_start(I2C_TypeDef *i2c_x, uint32_t recieve, uint8_t addr_dev,
// uint8_t cmd_dev, uint8_t *buff, uint32_t buff_len, uint32_t EVENT_ID);

void i2c_read(I2C_TypeDef *i2c_x, uint8_t device_addr, uint8_t *buff,
              uint32_t n_bytes, scheduler_event_t cb_event);

void i2c_write(I2C_TypeDef *i2c_x, uint8_t device_addr, uint8_t *buff,
               uint32_t n_bytes, scheduler_event_t cb_event);

// void i2c_stop(I2C_TypeDef *i2c_x);

// IRQ Handlers
void I2C0_IRQHandler(void);

#endif /* SRC_HEADER_FILES_I2C_H_ */
