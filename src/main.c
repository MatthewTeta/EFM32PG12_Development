/**
 * @file
 * @brief Simple LED Blink Demo for SLSTK3402A
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 */

#include "main.h"

int main(void) {
  uint32_t events;

  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_DEFAULT;
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;

  /* Chip errata */
  CHIP_Init();

  /* Init DCDC regulator and HFXO with kit specific parameters */
  /* Init DCDC regulator and HFXO with kit specific parameters */
  /* Initialize DCDC. Always start in low-noise mode. */
  EMU_EM23Init_TypeDef em23Init = EMU_EM23INIT_DEFAULT;
  EMU_DCDCInit(&dcdcInit);
  em23Init.vScaleEM23Voltage = emuVScaleEM23_LowPower;
  EMU_EM23Init(&em23Init);
  CMU_HFXOInit(&hfxoInit);

  /* Switch HFCLK to HFRCO and disable HFRCO */
  //  CMU_HFRCOBandSet(MCU_HFXO_FREQ);          // Set main CPU oscillator
  //  frequency
  // Need faster clock for i2c
  CMU_HFRCOBandSet(MCU_HFRCO_FREQ);
  CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFRCO);
  CMU_OscillatorEnable(cmuOsc_HFXO, false, false);

  /* Call application program to open / initialize all required peripheral */
  app_peripheral_setup();

  /* Infinite blink loop */
  while (1) {
    //    EMU_EnterEM1();
    //    EMU_EnterEM2(true);
    // Service scheduler
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();

    events = get_scheduled_events();
    //    remove_scheduled_event(~0);   // Remove all events as we will handle
    //    them all here
    if (!events)
      enter_sleep(); // Enter sleep mode based on peripheral usage
    CORE_EXIT_CRITICAL();

    // LETIMER Events
    if (events & LETIMER0_COMP0_CB) {
      EFM_ASSERT(false);
      // Clear event
      remove_scheduled_event(LETIMER0_COMP0_CB);
      EFM_ASSERT(!(get_scheduled_events() & LETIMER0_COMP0_CB));
      // Call underflow event handler
      scheduled_letimer0_comp0_cb();
    }
    if (events & LETIMER0_COMP1_CB) {
      // Clear event
      remove_scheduled_event(LETIMER0_COMP1_CB);
      EFM_ASSERT(!(get_scheduled_events() & LETIMER0_COMP1_CB));
      // Call underflow event handler
      scheduled_letimer0_comp1_cb();
    }
    if (events & LETIMER0_UF_CB) {
      // Clear event
      remove_scheduled_event(LETIMER0_UF_CB);
      EFM_ASSERT(!(get_scheduled_events() & LETIMER0_UF_CB));
      // Call underflow event handler
      scheduled_letimer0_uf_cb();
    }
    // Button Events
    if (events & GPIO_EVEN_IRQ_CB) {
      // Clear event
      remove_scheduled_event(GPIO_EVEN_IRQ_CB);
      EFM_ASSERT(!(get_scheduled_events() & GPIO_EVEN_IRQ_CB));
      scheduled_gpio_even_irq_cb();
    }
    if (events & GPIO_ODD_IRQ_CB) {
      // Clear event
      remove_scheduled_event(GPIO_ODD_IRQ_CB);
      EFM_ASSERT(!(get_scheduled_events() & GPIO_ODD_IRQ_CB));
      scheduled_gpio_odd_irq_cb();
    }
    // SI7021_Read Event
    if (events & SI7021_HUMIDITY_CB) {
      remove_scheduled_event(SI7021_HUMIDITY_CB);
      EFM_ASSERT(!(get_scheduled_events() & SI7021_HUMIDITY_CB));
      scheduled_si7021_read_humidity_cb();
    }
    if (events & SI7021_TEMPERATURE_CB) {
      remove_scheduled_event(SI7021_TEMPERATURE_CB);
      EFM_ASSERT(!(get_scheduled_events() & SI7021_TEMPERATURE_CB));
      scheduled_si7021_read_temperature_cb();
    }
    if (events & SHTC3_READ_CB) {
      remove_scheduled_event(SHTC3_READ_CB);
      EFM_ASSERT(!(get_scheduled_events() & SHTC3_READ_CB));
      scheduled_shtc3_read_cb();
    }
  }
}
