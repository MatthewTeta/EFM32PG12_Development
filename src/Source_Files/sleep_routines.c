/*
 * sleep_routines.c
 *
 *  Created on: Oct 3, 2022
 *      Author: Matthew Teta
 */

#include "sleep_routines.h"

/**
 * Private variables
 */
static uint32_t lowest_energy_mode[MAX_ENERGY_MODES];

void sleep_open(void) {
  // Initialize lowest_energy_modes array to all zeros
  int i;
  for (i = 0; i < MAX_ENERGY_MODES; i++)
    lowest_energy_mode[i] = 0;
}

void sleep_block_mode(uint32_t EM) {
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  EFM_ASSERT(EM < MAX_ENERGY_MODES);
  lowest_energy_mode[EM]++;
  // Allow 4 peripherals maximum
//  EFM_ASSERT(lowest_energy_mode[EM] < 5);

  CORE_EXIT_CRITICAL();
}

void sleep_unblock_mode(uint32_t EM) {
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  EFM_ASSERT(EM < MAX_ENERGY_MODES);
  lowest_energy_mode[EM]--;
  // Programmer error if asserted (all block statements must have corresponding unblock)
  EFM_ASSERT(lowest_energy_mode[EM] >= 0);

  CORE_EXIT_CRITICAL();
}

void enter_sleep(void) {
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();

  uint32_t mode = current_block_energy_mode();
  // Enter the corresponding mode (just under the sleepiest blocked mode)
  switch (mode) {
    case EM0:
      break;
    case EM1:
      break;
    case EM2:
      EMU_EnterEM1();
      break;
    case EM3:
      EMU_EnterEM2(true);
      break;
    case EM4:
      EMU_EnterEM3(true);
      break;
    default:
      break;
  }

  CORE_EXIT_CRITICAL();
}

uint32_t current_block_energy_mode(void) {
  uint32_t mode;
  // Select lowest non-blocked energy mode
  for (mode = 0; mode < MAX_ENERGY_MODES && lowest_energy_mode[mode] == 0; mode++);
  return mode;
}
