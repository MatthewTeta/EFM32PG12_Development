
#include "em_assert.h"
#include "em_cmu.h"
#include "em_leuart.h"
#include <stddef.h>

#include "scheduler.h"
#include "sleep_routines.h"

#ifndef LEUART_HG
#define LEUART_HG

#define LEUART_EM_BLOCK EM2

typedef struct {
  LEUART_Init_TypeDef init;
  uint32_t            RX_LOC;
  uint32_t            TX_LOC;
  bool                RX_EN;
  bool                TX_EN;
  uint32_t           *rx_byte;
  scheduler_event_t   rx_cb_event;
} leuart_open_t;

void leuart_open(LEUART_TypeDef *leuart_x, leuart_open_t *o);

// Only implementing TX at the moment (not sure what to do with bytes received
// yet)
void leuart_tx_buff(LEUART_TypeDef *leuart_x, uint8_t *buff, uint32_t len,
                    scheduler_event_t cb_event);

#endif
