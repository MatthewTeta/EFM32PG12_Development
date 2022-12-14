#include "CC2640.h"

// Private Static variables
static bool            CC2640_open_ind   = false;
static LEUART_TypeDef *CC2640_leuart_x   = NULL;
static bool            cc2640_data_valid = false;
static uint8_t         cc2640_rx_byte    = 0;

void CC2640_open(LEUART_TypeDef *leuart_x, scheduler_event_t rx_cb_event) {
  CC2640_leuart_x = leuart_x;

  // Wait for peripheral to be ready after power on
  timer_delay(CC2640_POW_MS);

  uint32_t TX_LOC;
  uint32_t RX_LOC;
  if (leuart_x == LEUART0) {
    TX_LOC = LEUART_ROUTELOC0_TXLOC_LOC18;
    RX_LOC = LEUART_ROUTELOC0_RXLOC_LOC18;
  } else
    EFM_ASSERT(false);

  leuart_open_t o = {
      .init =
          {
              .baudrate = CC2640_LEUART_BAUD,
              .databits = CC2640_LEUART_DATA_BITS,
              .enable   = CC2640_LEUART_ENABLE,
              .parity   = CC2640_LEUART_PARITY,
              .refFreq  = CC2640_LEUART_REF_FREQ,
              .stopbits = CC2640_LEUART_STOP_BITS,
          },
      .INVERT      = CC2640_LEUART_INVERT,
      .TX_LOC      = TX_LOC,
      .RX_LOC      = RX_LOC,
      .TX_EN       = true,
      .RX_EN       = true,
      .rx_byte     = &cc2640_rx_byte,
      .rx_cb_event = rx_cb_event,
  };

  leuart_open(leuart_x, &o);

  // Indicate successful setup
  CC2640_open_ind = true;

  // Enable the module by sending 1024 bytes
  uint8_t  z = 0xAA;
  uint32_t i;
  for (i = 0; i < CC2640_START_TX_LEN; ++i)
    CC2640_tx_buff(&z, 1, 0);

  cc2640_data_valid = true;
}

uint32_t cc2640_get_char(uint8_t *c) {
  if (!cc2640_data_valid)
    return -1;
  *c = cc2640_rx_byte;
  return 0;
}

void CC2640_tx_buff(uint8_t *buff, uint32_t len, scheduler_event_t cb_event) {
  if (!CC2640_open_ind)
    EFM_ASSERT(false);

  // Pass call to leuart driver
  leuart_tx_buff(CC2640_leuart_x, buff, len, cb_event);
}

// TODO
// void CC2640_request_conn_status(void) {}

void CC2640_request_help(void) {
  static uint8_t cmd_help[] = "AT";
  CC2640_tx_buff(cmd_help, (sizeof(cmd_help) / sizeof(uint8_t)) - 1, 0);
}
