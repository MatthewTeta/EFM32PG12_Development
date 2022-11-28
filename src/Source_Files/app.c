
/**
 * @file app.c
 * @author Matthew Teta
 * @date 09/18/2022
 * @brief Custom app config.
 */
//***********************************************************************************
// Include files
//***********************************************************************************
#include "app.h"
#include <stdio.h>
#include <string.h>

//***********************************************************************************
// defined files
//***********************************************************************************

//***********************************************************************************
// Static / Private Variables
//***********************************************************************************

//***********************************************************************************
// Private functions
//***********************************************************************************

static void app_letimer_pwm_open(float period, float act_period,
                                 uint32_t out0_route, uint32_t out1_route);

//***********************************************************************************
// Global functions
//***********************************************************************************

/**
 * @brief
 *   Entry point for all custom peripheral initialization.
 *
 * @details
 *   Call This function in the main file before attempting to use any of the
 *configured peripherals
 *
 * @note
 *   This function takes no arguments because this module will contain all of
 *the details for the custom configuration. This keeps the details out of the
 *other code modules.
 *
 */
void app_peripheral_setup(void) {
  cmu_open();
  gpio_open(GPIO_EVEN_IRQ_CB, GPIO_ODD_IRQ_CB);
  app_letimer_pwm_open(PWM_PER, PWM_ACT_PER, PWM_ROUTE_0, PWM_ROUTE_1);
  scheduler_open();
  sleep_open();
  si7021_i2c_open(SI7021_I2C);
  shtc3_i2c_open(SHTC3_I2C);
  CC2640_open(CC2640_LEUART, CC2640_RX_CB);

  letimer_start(LETIMER0, true); // letimer_start will inform the LETIMER0
                                 // peripheral to begin counting.
}

/**
 * @brief
 *   Set up the LETIMER to generate a pwm signal
 *
 * @details
 *   Generate custom LETIMER peripheral config to pass to the generic LETIMER
 *   module which will output a PWM with period, duty cycle, and output routing
 *
 * @note
 *   This will use LETIMER0. There are two possible outputs for the resulting
 *   signal
 *
 * @param[in] period
 *   PWM period in seconds
 *
 * @param[in] act_period
 *   Duty cycle HIGH time in seconds
 *
 * @param[in] out0_route
 *   This is the route number from the board specific (family) data sheet for
 *   the first output
 *
 * @param[in] out1_route
 *   Route number for the second output
 *
 */
void app_letimer_pwm_open(float period, float act_period, uint32_t out0_route,
                          uint32_t out1_route) {
  // Initializing LETIMER0 for PWM operation by creating the
  // letimer_pwm_struct and initializing all of its elements
  APP_LETIMER_PWM_TypeDef letimer_pwm_struct = {
      .active_period    = act_period,
      .debugRun         = false,
      .enable           = false,
      .out_pin_0_en     = false,
      .out_pin_1_en     = false,
      .out_pin_route0   = out0_route,
      .out_pin_route1   = out1_route,
      .period           = period,
      .comp0_isr_enable = false,
      .comp0_cb         = LETIMER0_COMP0_CB,
      .comp1_isr_enable = true,
      .comp1_cb         = LETIMER0_COMP1_CB,
      .uf_isr_enable    = true,
      .uf_cb            = LETIMER0_UF_CB,
  };

  letimer_pwm_open(LETIMER0, &letimer_pwm_struct);
}

void scheduled_letimer0_comp0_cb(void) {}

void scheduled_letimer0_comp1_cb(void) {}

void scheduled_letimer0_uf_cb(void) {
  // Begin si7021 read
  si7021_i2c_begin_read_humidity(SI7021_HUMIDITY_CB);
  si7021_i2c_begin_read_temperature(SI7021_TEMPERATURE_CB);
  shtc3_i2c_begin_read(SHTC3_READ_CB);
}

void scheduled_gpio_even_irq_cb(void) {
  EFM_ASSERT(true);
  //  uint32_t em = current_block_energy_mode();
  // Decrement sleep mode to unblock
  //  if (em >= 0) {
  //    sleep_unblock_mode(em);
  //
  //  } else {
  //    sleep_unblock_mode(EM4);
  //  }
}

void scheduled_gpio_odd_irq_cb(void) {
  EFM_ASSERT(true);
  //  uint32_t em = current_block_energy_mode();
  // Increment sleep mode block
  //  if (em > 0) {
  //    sleep_block_mode(em - 1);
  //  } else {
  //    sleep_block_mode(EM4);
  //  }
}

void scheduled_si7021_read_humidity_cb(void) {
  float rv = si7021_get_humidity();
  if (rv < 0 || rv > 100)
    return;
  char s[32];
  int  len = sprintf(s, "%f\n", rv);
  if (len < 0)
    EFM_ASSERT(false);
  //  CC2640_tx_buff((uint8_t *)s, (uint32_t)len, 0);
  CC2640_request_help();
  EFM_ASSERT(true);
}

void scheduled_si7021_read_temperature_cb(void) {
  // float rv = si7021_get_temperature();
  // leuart_tx_buff()
  EFM_ASSERT(true);
}

void scheduled_shtc3_read_cb(void) {
  float temperature, humidity;
  if (!shtc3_get_temperature_and_humidity(&temperature, &humidity))
    return;
  // Convert to F
  temperature = 32 + 1.8 * temperature;
  // TODO: Do something with the result
  if (temperature > 20 && temperature < 100)
    return;
  EFM_ASSERT(true);
}

void scheduled_cc2640_rx_event(void) {
  static uint8_t  rx_buff[CC2640_RX_BUFF_LEN] = {0};
  static uint8_t  c                           = 0;
  static uint32_t i                           = 0;
  if (!cc2640_get_char(&c))
    return;
  if (i < CC2640_RX_BUFF_LEN) {
    rx_buff[i] = c;
    i += 1;
  } else {
    memset(rx_buff, 0, CC2640_RX_BUFF_LEN);
  }
}
