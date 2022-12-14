
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
  uint32_t            RX_LOC;
  uint32_t            TX_LOC;
  bool                RX_EN;
  bool                TX_EN;
  bool                INVERT;
  uint8_t            *rx_byte;
  scheduler_event_t   rx_cb_event;
  LEUART_Init_TypeDef init;
} leuart_open_t;

/**
 * @brief Initialize the LEUART peripheral for use
 *
 * @param leuart_x
 * @param o Input parameters
 */
void leuart_open(LEUART_TypeDef *leuart_x, leuart_open_t *o);

/**
 * @brief The one and only method for initializing a TX data operation on LEUART
 *
 * @param leuart_x which leuart peripheral
 * @param buff data to transmit
 * @param len num bytes for tx
 * @param cb_event optional callback event (use 0 for none)
 */
void leuart_tx_buff(LEUART_TypeDef *leuart_x, uint8_t *buff, uint32_t len,
                    scheduler_event_t cb_event);

#endif
