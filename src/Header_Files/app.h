//***********************************************************************************
// Include files
//***********************************************************************************
#ifndef APP_HG
#define APP_HG

/* System include statements */

/* Silicon Labs include statements */
#include "em_assert.h"
#include "em_cmu.h"

/* The developer's include statements */
#include "SI7021.h"
#include "brd_config.h"
#include "cmu.h"
#include "gpio.h"
#include "letimer.h"
#include "leuart.h"
#include "scheduler.h"
#include "sleep_routines.h"

//***********************************************************************************
// defined files
//***********************************************************************************
#define PWM_PER     0.5   // PWM period in seconds
#define PWM_ACT_PER 0.002 // PWM active period in seconds

// EVENTS
#define LETIMER0_COMP0_CB     (1 << 0)
#define LETIMER0_COMP1_CB     (1 << 1)
#define LETIMER0_UF_CB        (1 << 2)
#define GPIO_EVEN_IRQ_CB      (1 << 3)
#define GPIO_ODD_IRQ_CB       (1 << 4)
#define SI7021_HUMIDITY_CB    (1 << 5)
#define SI7021_TEMPERATURE_CB (1 << 6)
#define SHTC3_READ_CB         (1 << 7)

//***********************************************************************************
// global variables
//***********************************************************************************

//***********************************************************************************
// function prototypes
//***********************************************************************************
void app_peripheral_setup(void);
void scheduled_letimer0_comp0_cb(void);
void scheduled_letimer0_comp1_cb(void);
void scheduled_letimer0_uf_cb(void);
void scheduled_gpio_even_irq_cb(void);
void scheduled_gpio_odd_irq_cb(void);
void scheduled_si7021_read_humidity_cb(void);
void scheduled_si7021_read_temperature_cb(void);
void scheduled_shtc3_read_cb(void);

#endif
