//***********************************************************************************
// Include files
//***********************************************************************************
#ifndef SCHEDULER_HG
#define SCHEDULER_HG

/* System include statements */
#include <stdint.h>

/* Silicon Labs include statements */
#include "em_assert.h"
#include "em_core.h"
#include "em_emu.h"

/* The developer's include statements */
//#include "sleep_routines.h"

typedef uint32_t scheduler_event_t;

//***********************************************************************************
// defined files
//***********************************************************************************

//***********************************************************************************
// global variables
//***********************************************************************************

//***********************************************************************************
// function prototypes
//***********************************************************************************

/**
 * @brief Initialize the scheduler for use
 *
 */
void scheduler_open(void);

// Push an event into the scheduler so it can be serviced elsewhere
void add_scheduled_event(uint32_t event);

// Remove an event from the bit vector
void remove_scheduled_event(uint32_t event);

// Check which events are waiting to be serviced
uint32_t get_scheduled_events(void);

#endif
