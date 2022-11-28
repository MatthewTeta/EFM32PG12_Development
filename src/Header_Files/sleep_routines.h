/*
 * sleep_routines.h
 *
 *  Created on: Oct 3, 2022
 *      Author: Matthew Teta
 */

#ifndef SRC_HEADER_FILES_SLEEP_ROUTINES_H_
#define SRC_HEADER_FILES_SLEEP_ROUTINES_H_

#include "em_assert.h"
#include "em_core.h"
#include "em_emu.h"

// I would define this as an enum
#define EM0              0
#define EM1              1
#define EM2              2
#define EM3              3
#define EM4              4
#define MAX_ENERGY_MODES 5

// Configure the sleep module
void sleep_open(void);

// Add 1 to the block for a sleep mode (used when performing an action)
void sleep_block_mode(uint32_t EM);

// Subtract 1 to the block for a sleep mode (used when action is finished)
void sleep_unblock_mode(uint32_t EM);

// Call this to change to the proper sleep mode (min which is not blocked)
void enter_sleep(void);

// Gets the current energy mode which is blocked
uint32_t current_block_energy_mode(void);

#endif /* SRC_HEADER_FILES_SLEEP_ROUTINES_H_ */
