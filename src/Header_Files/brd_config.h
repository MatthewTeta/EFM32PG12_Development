#ifndef BRD_CONFIG_HG
#define BRD_CONFIG_HG

//***********************************************************************************
// Include files
//***********************************************************************************
/* System include statements */

/* Silicon Labs include statements */
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_i2c.h"
#include "em_leuart.h"

/* The developer's include statements */

//***********************************************************************************
// defined files
//***********************************************************************************

// GPIO pin setup
#define STRONG_DRIVE

// LED 0 pin is
#define LED0_PORT     gpioPortF
#define LED0_PIN      04u
#define LED0_DEFAULT  false // Default false (0) = off, true (1) = on
#define LED0_GPIOMODE gpioModePushPull

// LED 1 pin is
#define LED1_PORT     gpioPortF
#define LED1_PIN      05u
#define LED1_DEFAULT  false // Default false (0) = off, true (1) = on
#define LED1_GPIOMODE gpioModePushPull

#ifdef STRONG_DRIVE
#define LED0_DRIVE_STRENGTH gpioDriveStrengthStrongAlternateStrong
#define LED1_DRIVE_STRENGTH gpioDriveStrengthStrongAlternateStrong
#else
#define LED0_DRIVE_STRENGTH gpioDriveStrengthWeakAlternateWeak
#define LED1_DRIVE_STRENGTH gpioDriveStrengthWeakAlternateWeak
#endif

// System Clock setup
//#define MCU_HFXO_FREQ     cmuHFRCOFreq_1M0Hz
#define MCU_HFRCO_FREQ cmuHFRCOFreq_26M0Hz

// LETIMER PWM Configuration
#define PWM_ROUTE_0 28
#define PWM_ROUTE_1 28

// Button Configuration
#define BUTTON_0_PORT   gpioPortF
#define BUTTON_0_PIN    06u
#define BUTTON_0_CONFIG gpioModeInput
#define BUTTON_1_PORT   gpioPortF
#define BUTTON_1_PIN    07u
#define BUTTON_1_CONFIG gpioModeInput

// Button Interrupt Configuration
#define BUTTON_0_INT_NUM     BUTTON_0_PIN
#define BUTTON_0_INT_RISING  false
#define BUTTON_0_INT_FALLING true
#define BUTTON_0_INT_ENABLE  true
#define BUTTON_1_INT_NUM     BUTTON_1_PIN
#define BUTTON_1_INT_RISING  false
#define BUTTON_1_INT_FALLING true
#define BUTTON_1_INT_ENABLE  true

// I2C GPIO Setup
#define SI7021_SCL_PORT                 gpioPortC
#define SI7021_SCL_PIN                  11
#define SI7021_SCL_GPIOMODE             gpioModeWiredAnd
#define SI7021_SDA_PORT                 gpioPortC
#define SI7021_SDA_PIN                  10
#define SI7021_SDA_GPIOMODE             gpioModeWiredAnd
#define SI7021_SENSOR_EN_PORT           gpioPortB
#define SI7021_SENSOR_EN_PIN            10
#define SI7021_SENSOR_EN_DRIVE_STRENGTH gpioDriveStrengthWeakAlternateWeak
#define SI7021_SENSOR_EN_GPIOMODE       gpioModePushPull
#define SI7021_I2C                      I2C0

// SHTC3 GPIO Config
#define SHTC3_SCL_PORT     gpioPortB
#define SHTC3_SCL_PIN      7
#define SHTC3_SCL_GPIOMODE gpioModeWiredAnd
#define SHTC3_SDA_PORT     gpioPortB
#define SHTC3_SDA_PIN      6
#define SHTC3_SDA_GPIOMODE gpioModeWiredAnd
#define SHTC3_I2C          I2C1

// BLE LEUART Setup
#define CC2640_LEUART LEUART0

//***********************************************************************************
// global variables
//***********************************************************************************

//***********************************************************************************
// function prototypes
//***********************************************************************************

#endif
