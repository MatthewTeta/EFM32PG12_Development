/**
 * @file gpio.c
 * @author Matthew Teta
 * @date 09/18/2022
 * @brief Custom GPIO config
 */

//***********************************************************************************
// Include files
//***********************************************************************************
#include "gpio.h"


//***********************************************************************************
// defined files
//***********************************************************************************


//***********************************************************************************
// Private variables
//***********************************************************************************
static uint32_t gpio_even_event_cb;
static uint32_t gpio_odd_event_cb;

//***********************************************************************************
// Private functions
//***********************************************************************************


//***********************************************************************************
// Global functions
//***********************************************************************************

/**
 * @brief
 *   Configure the GPIO ports in use in this application (custom)
 *
 * @details
 *   Enables the GPIO clock and sets the onboard LED pins such that they can be
 *   driven internally by the LETIMER peripheral
 */
void gpio_open(uint32_t gpio_even_event, uint32_t gpio_odd_event){
  gpio_even_event_cb  = gpio_even_event;
  gpio_odd_event_cb   = gpio_odd_event;

  CMU_ClockEnable(cmuClock_GPIO, true);

  // Configure LED pins
  GPIO_PinModeSet(LED0_PORT, LED0_PIN, LED0_GPIOMODE, LED0_DEFAULT);
  GPIO_PinModeSet(LED1_PORT, LED1_PIN, LED1_GPIOMODE, LED1_DEFAULT);
  GPIO_DriveStrengthSet(LED0_PORT, LED0_DRIVE_STRENGTH);
  GPIO_DriveStrengthSet(LED1_PORT, LED1_DRIVE_STRENGTH);

  // Configure Buttons
  GPIO_PinModeSet(BUTTON_0_PORT, BUTTON_0_PIN, BUTTON_0_CONFIG, false);
  GPIO_PinModeSet(BUTTON_1_PORT, BUTTON_1_PIN, BUTTON_1_CONFIG, false);
  // Configure Interrupts for buttons
  GPIO_ExtIntConfig(BUTTON_0_PORT,
                    BUTTON_0_PIN,
                    BUTTON_0_INT_NUM,
                    BUTTON_0_INT_RISING,
                    BUTTON_0_INT_FALLING,
                    BUTTON_0_INT_ENABLE);
  GPIO_ExtIntConfig(BUTTON_1_PORT,
                    BUTTON_1_PIN,
                    BUTTON_1_INT_NUM,
                    BUTTON_1_INT_RISING,
                    BUTTON_1_INT_FALLING,
                    BUTTON_1_INT_ENABLE);

  // Enable Button Interrupts with the NVIC
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);

  // Configure GPIO for SI7021 I2C
  GPIO_DriveStrengthSet(SI7021_SENSOR_EN_PORT, SI7021_SENSOR_EN_DRIVE_STRENGTH);
  GPIO_PinModeSet(SI7021_SENSOR_EN_PORT,  SI7021_SENSOR_EN_PIN, SI7021_SENSOR_EN_GPIOMODE,  true);
  GPIO_PinModeSet(SI7021_SCL_PORT,        SI7021_SCL_PIN,       SI7021_SCL_GPIOMODE,        true);
  GPIO_PinModeSet(SI7021_SDA_PORT,        SI7021_SDA_PIN,       SI7021_SDA_GPIOMODE,        true);

}

// IRQ handlers
void GPIO_EVEN_IRQHandler(void) {
  uint32_t IF;
  // Read interrupt flags
  IF = GPIO->IF & GPIO->IEN;
  // Clear interrupt flags
  GPIO->IFC = IF;
  // Handle even interrupt
  if (IF) {
      // Make sure interrupt got cleared
//      EFM_ASSERT(!(GPIO->IF & GPIO_IF_EXT_DEFAULT));
      add_scheduled_event(gpio_even_event_cb);
  }
}

void GPIO_ODD_IRQHandler(void) {
  uint32_t IF;
  // Read interrupt flags
  IF = GPIO->IF & GPIO->IEN;
  // Clear interrupt flags
  GPIO->IFC = IF;
  // Handle odd interrupt
  if (IF) {
      // Make sure interrupt got cleared
//      EFM_ASSERT(!(GPIO->IF & GPIO_IF_EXT_DEFAULT));
      add_scheduled_event(gpio_odd_event_cb);
  }
}
