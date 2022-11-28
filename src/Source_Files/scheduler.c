/*
 * scheduler.c
 *
 *  Created on: Sep 29, 2022
 *      Author: Matthew Teta
 */
#include "scheduler.h"

static uint32_t event_scheduled;

void scheduler_open(void) { event_scheduled = 0; }

void add_scheduled_event(uint32_t event) {
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  event_scheduled |= event;
  CORE_EXIT_CRITICAL();
}

void remove_scheduled_event(uint32_t event) {
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  event_scheduled &= ~event;
  CORE_EXIT_CRITICAL();
}

uint32_t get_scheduled_events(void) { return event_scheduled; }
