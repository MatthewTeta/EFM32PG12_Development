
#include "leuart.h"

typedef struct {
  LEUART_TypeDef   *leuart_x;
  bool              busy;
  uint8_t          *buff;
  uint32_t          i;
  uint32_t          len;
  uint32_t         *rx_byte;
  scheduler_event_t tx_cb_event;
  scheduler_event_t rx_cb_event;
} leuart_sm_t;

// Private static
// static bool leuart_opened = false;

// Private static functions
static void _leuart_bus_reset(LEUART_TypeDef *leuart_x);
static volatile leuart_sm_t             *
_get_leuart_tx_sm_from_leuart_td(LEUART_TypeDef *leuart_x);
static void _leuart_rx_sm_handle_RXDATAV(volatile leuart_sm_t *sm);
static void _leuart_tx_sm_handle_TXC(volatile leuart_sm_t *sm);
static void _leuart_tx_sm_handle_TXBL(volatile leuart_sm_t *sm);

// sm singleton for each leuart on board
static volatile leuart_sm_t leuart0_sm = {
    .leuart_x = LEUART0,
    .busy     = false,
    .buff     = NULL,
    .i        = 0,
    .len      = 0,
};

void leuart_open(LEUART_TypeDef *leuart_x, leuart_open_t *o) {
  volatile leuart_sm_t *sm = _get_leuart_tx_sm_from_leuart_td(leuart_x);

  // Check if any job is running currently
  while (sm->busy)
    ;

  _leuart_bus_reset(leuart_x);

  // Enable clock to peripheral
  if (leuart_x == LEUART0) {
    CMU_ClockEnable(cmuClock_LEUART0, true);
  } else {
    EFM_ASSERT(false);
  }

  // Ensure that the clock is enabled properly
  if ((leuart_x->IF & 0x01) == 0) {
    leuart_x->IFS = 0x01;
    EFM_ASSERT(leuart_x->IF & 0x01);
    leuart_x->IFC = 0x01;
  } else {
    leuart_x->IFC = 0x01;
    EFM_ASSERT(!(leuart_x->IF & 0x01));
  }

  // Initialize leuart
  LEUART_Init(leuart_x, &o->init);
  // Set GPIO Routes
  leuart_x->ROUTELOC0 = o->RX_LOC | o->TX_LOC;
  leuart_x->ROUTEPEN = (uint32_t)(((o->TX_EN & 0x01) << 1) | (o->RX_EN & 0x01));

  // Setup RX buff location
  sm->rx_byte = o->rx_byte;
  if (!o->RX_EN) {
    sm->rx_byte     = NULL;
    sm->rx_cb_event = o->rx_cb_event;
  }

  // Enable Interrups
  leuart_x->IFC = (uint32_t)~0;
  leuart_x->IEN = LEUART_IEN_RXDATAV;

  // Enable LEUART Interrupts with the NVIC
  NVIC_EnableIRQ(LEUART0_IRQn);
}

void leuart_tx_buff(LEUART_TypeDef *leuart_x, uint8_t *buff, uint32_t len,
                    scheduler_event_t tx_cb_event) {
  volatile leuart_sm_t *sm = _get_leuart_tx_sm_from_leuart_td(leuart_x);
  // Block while peripheral transmit is busy
  while (sm->busy)
    ;
  EFM_ASSERT(leuart_x->STATUS & LEUART_STATUS_TXIDLE);
  // Begin transaction by calling equivallent IRQ handler function
  sleep_block_mode(LEUART_EM_BLOCK);
  // Fill tx sm
  sm->busy        = true;
  sm->buff        = buff;
  sm->i           = 0;
  sm->len         = len;
  sm->tx_cb_event = tx_cb_event;

  // Cause the start of the transaction to occur
  leuart_x->IEN = LEUART_IEN_RXDATAV | LEUART_IEN_TXC;
  _leuart_tx_sm_handle_TXC(sm);
}

static void _leuart_bus_reset(LEUART_TypeDef *leuart_x) {
  LEUART_Reset(leuart_x);
}

static volatile leuart_sm_t *
_get_leuart_tx_sm_from_leuart_td(LEUART_TypeDef *leuart_x) {
  if (leuart_x == LEUART0)
    return &leuart0_sm;
  EFM_ASSERT(false);
  return NULL;
}

// IRQ Handlers
void LEUART0_IRQHandler(void) {
  LEUART_TypeDef *leuart_x = LEUART0;
  // Handle LEUART Events
  uint32_t IF;
  // Read interrupt flags
  IF = leuart_x->IF & leuart_x->IEN;
  // Clear interrupt flags
  leuart_x->IFC = IF;
  // Get corresponding tx_sm
  volatile leuart_sm_t *sm = _get_leuart_tx_sm_from_leuart_td(leuart_x);
  // LEUART_IEN_RXDATAV | LEUART_IEN_TXBL | LEUART_IEN_TXC
  // TXBL (TX Level) | TXC (TX Complete)
  if (IF & LEUART_IEN_RXDATAV) {
    _leuart_rx_sm_handle_RXDATAV(sm);
  }
  if (IF & LEUART_IF_TXBL) {
//    _leuart_tx_sm_handle_TXBL(sm);
  }
  if (IF & LEUART_IF_TXC) {
    _leuart_tx_sm_handle_TXC(sm);
  }
}

static void _leuart_rx_sm_handle_RXDATAV(volatile leuart_sm_t *sm) {
  // Set rx_byte and tell scheduler to handle the byte
  if (sm->rx_byte != NULL) {
    *sm->rx_byte = sm->leuart_x->RXDATA;
    add_scheduled_event(sm->rx_cb_event);
  }
}

/**
 * @brief Used to handle the TXC interrupt from the LEUART. Ignored unless all
 * data has been transmitted, then it's used to complete the transaction and set
 * the busy state of the sm to false
 *
 * @param sm Instance of tx state machine for the given leuart
 */
static void _leuart_tx_sm_handle_TXC(volatile leuart_sm_t *sm) {
  if (!sm->busy)
    return;
  if (sm->i < sm->len) {

      sm->leuart_x->TXDATA = sm->buff[sm->i];
      sm->i += 1;
  } else {
    sm->leuart_x->IEN = LEUART_IEN_RXDATAV;
    sm->busy          = false;
    sleep_unblock_mode(LEUART_EM_BLOCK);
    add_scheduled_event(sm->tx_cb_event);
  }
}

/**
 * @brief Handles the TXL interrupt. Sends the data until there is no more to
 * send as indicated by sm->len.
 *
 * @param sm Instance of tx state machine for the given leuart
 */
static void _leuart_tx_sm_handle_TXBL(volatile leuart_sm_t *sm) {
//  if (!sm->busy)
//    return;
//  if (sm->i < sm->len) {
//    sm->leuart_x->TXDATA = sm->buff[sm->i];
//    sm->i += 1;
//  }
}
