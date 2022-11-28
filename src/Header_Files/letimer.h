//***********************************************************************************
// Include files
//***********************************************************************************
#ifndef LETIMER_HG
#define LETIMER_HG

/* System include statements */

/* Silicon Labs include statements */
#include "em_assert.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_letimer.h"

/* The developer's include statements */
#include "scheduler.h"
#include "sleep_routines.h"

//***********************************************************************************
// defined files
//***********************************************************************************
#define LETIMER_HZ 1000 // Utilizing ULFRCO oscillator for LETIMERs
#define LETIMER_EM                                                             \
  EM4 // Block using EM4 (LE3 is the lowest possible mode for LETIMER)

//***********************************************************************************
// global variables
//***********************************************************************************
typedef struct {
  bool     debugRun;       // True = keep LETIMER running will halted
  bool     enable;         // enable the LETIMER upon completion of open
  uint32_t out_pin_route0; // out 0 route to gpio port/pin
  uint32_t out_pin_route1; // out 1 route to gpio port/pin
  bool     out_pin_0_en;   // enable out 0 route
  bool     out_pin_1_en;   // enable out 1 route
  float    period;         // seconds
  float    active_period;  // seconds
  bool     comp0_isr_enable;
  uint32_t comp0_cb;
  bool     comp1_isr_enable;
  uint32_t comp1_cb;
  bool     uf_isr_enable;
  uint32_t uf_cb;
} APP_LETIMER_PWM_TypeDef;

//***********************************************************************************
// function prototypes
//***********************************************************************************

// Configure the LETIMER peripheral for use
void letimer_pwm_open(LETIMER_TypeDef         *letimer,
                      APP_LETIMER_PWM_TypeDef *app_letimer_struct);

// Start or stop the LETIMER counting
void letimer_start(LETIMER_TypeDef *letimer, bool enable);

// IRQ Handler Overload
void LETIMER0_IRQHandler(void);

#endif
