#include "HW_delay.h"
#include "em_assert.h"
#include "leuart.h"

#ifndef CC2640_HG
#define CC2640_HG

// This value is not listed in data sheet so I'm being conservative
#define CC2640_POW_MS           3000
#define CC2640_START_TX_LEN     1024
#define CC2640_LEUART_BAUD      9600
#define CC2640_LEUART_DATA_BITS leuartDatabits8
#define CC2640_LEUART_ENABLE    leuartEnable
#define CC2640_LEUART_PARITY    LEUART_CTRL_PARITY_NONE
#define CC2640_LEUART_REF_FREQ  0
#define CC2640_LEUART_STOP_BITS LEUART_CTRL_STOPBITS_ONE
#define CC2640_LEUART_INVERT    false

/**
 * @brief This sets up the leaurt peripheral for use with the CC2640 (HM-15) BLE module
 * 
 * @param leuart_x Decide which LEUART peripheral to use
 * @param rx_cb_event Scheduler event to handle bytes as they are recieved in application layer
 */
void CC2640_open(LEUART_TypeDef *leuart_x, scheduler_event_t rx_cb_event);

/**
 * @brief Sends the most recent charachter received through the function arg
 *
 * @param c Return value pass by reference
 * @return uint32_t Returns non-zero if there is an error
 */
uint32_t cc2640_get_char(uint8_t *c);

// void leuart_tx_buff(LEUART_TypeDef *leuart_x, uint8_t *buff, uint32_t len,
// scheduler_event_t cb_event);
/**
 * @brief Transmit buffer data (essentially just forwards params to leuart
 * function call for tx)
 *
 * @param buff data to transmit
 * @param len length of buff
 * @param cb_event user callback in scheduler (use zero for no callback)
 */
void CC2640_tx_buff(uint8_t *buff, uint32_t len, scheduler_event_t cb_event);

// void CC2640_request_conn_status(void);

/**
 * @brief Send special help command to BLE device
 * 
 */
void CC2640_request_help(void);

#endif
