//***********************************************************************************
// Include files
//***********************************************************************************
#ifndef GPIO_HG
#define GPIO_HG

/* System include statements */


/* Silicon Labs include statements */
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_assert.h"

/* The developer's include statements */
#include "brd_config.h"
#include "scheduler.h"

//***********************************************************************************
// defined files
//***********************************************************************************

//***********************************************************************************
// global variables
//***********************************************************************************


//***********************************************************************************
// function prototypes
//***********************************************************************************
void gpio_open(uint32_t gpio_even_event, uint32_t gpio_odd_event);

// IRQ Handlers
void GPIO_EVEN_IRQHandler(void);
void GPIO_ODD_IRQHandler(void);

#endif
