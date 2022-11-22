
#include "i2c.h"
#include <stddef.h>
// #include "queue.h"

// State Machine States
// typedef enum
// {
//   I2C_SM_STATE_INIT,
//   I2C_SM_STATE_SETREG,
//   I2C_SM_STATE_WAITSLAVE,
//   I2C_SM_STATE_RXBUFF,
//   I2C_SM_STATE_STOP,
// } I2C_SM_STATES;

// I2C Internal State machine for determining what to do in the I2C_IRQHandler
// typedef struct
// {
//   I2C_TypeDef *i2c_x;
//   I2C_SM_STATES state;
//   uint32_t busy;
//   uint32_t recieve;
//   uint32_t rx_buff_len;
//   uint8_t *rx_buff;
//   uint32_t rx_buff_offset;
//   uint8_t addr_dev;
//   uint8_t cmd_dev;
//   uint32_t EVENT_ID;
// } i2c_sm;

// static i2c_sm_t I2C0_sm = {
//     .i2c_x = I2C0,
//     .state = I2C_SM_STATE_INIT,
//     .busy = false,
//     .recieve = true,
//     .rx_buff_len = 0,
//     .rx_buff = 0,
//     .rx_buff_offset = 0,
//     .addr_dev = 0,
//     .cmd_dev = 0,
//     .EVENT_ID = 0,
// };

// static i2c_sm_t I2C1_sm = {
//     .i2c_x          =   I2C0,
//     .state          =   I2C_SM_STATE_INIT,
//     .busy           =   false,
//     .recieve        =   true,
//     .rx_buff_len    =   0,
//     .rx_buff        =   0,
//     .rx_buff_offset =   0,
//     .addr_dev       =   0,
//     .cmd_dev        =   0,
//     .EVENT_ID       =   0,
// };

// Rev B
typedef enum {
  I2C_JOB_TYPE_READ,
  I2C_JOB_TYPE_WRITE,
  // I2C_JOB_TYPE_STOP,
} I2C_JOB_TYPE;

typedef struct {
  I2C_JOB_TYPE      type;
  uint8_t          *buff;
  uint32_t          len;
  uint32_t          i;
  uint8_t           device_addr;
  scheduler_event_t cb_event;
} i2c_job_t;

typedef struct {
  I2C_TypeDef *i2c_x;
  i2c_job_t    job;
  bool         busy;
} i2c_sm_t;

// Private static singletons
static volatile i2c_sm_t i2c0_sm = {
    .i2c_x = I2C0,
    .job =
        {
            .type        = I2C_JOB_TYPE_READ,
            .buff        = NULL,
            .len         = 0,
            .i           = 0,
            .device_addr = 0x00,
            .cb_event    = 0,
        },
    .busy = false,
};

static volatile i2c_sm_t i2c1_sm = {
    .i2c_x = I2C1,
    .job =
        {
            .type        = I2C_JOB_TYPE_READ,
            .buff        = NULL,
            .len         = 0,
            .i           = 0,
            .device_addr = 0x00,
            .cb_event    = 0,
        },
    .busy = false,
};

// Job queues
// queue_t *i2c0_q;
// queue_t *i2c1_q;

/**
 * PRIVATE FUNCTIONS
 */
static void               _i2c_bus_reset(I2C_TypeDef *i2c_x);
static volatile i2c_sm_t *_get_sm_from_i2c(I2C_TypeDef *i2c_x);
// Async (ISR)
static void _i2c_sm_handle_ack(volatile i2c_sm_t *sm);
static void _i2c_sm_handle_nack(volatile i2c_sm_t *sm);
static void _i2c_sm_handle_rxdatav(volatile i2c_sm_t *sm);
static void _i2c_sm_handle_mstop(volatile i2c_sm_t *sm);

/**
 * Function Definitions
 */

/**
 * @brief Setup the i2c interface
 */
void i2c_open(I2C_TypeDef *i2c_x, I2C_open_type *i2c_open_struct) {
  // Check if any job is running currently
  while (i2c0_sm.busy || i2c1_sm.busy)
    ;

  // Enable clock to peripheral
  if (i2c_x == I2C0) {
    CMU_ClockEnable(cmuClock_I2C0, true);
  } else if (i2c_x == I2C1) {
    CMU_ClockEnable(cmuClock_I2C1, true);
  }

  // Ensure that the clock is enabled properly
  if ((i2c_x->IF & 0x01) == 0) {
    i2c_x->IFS = 0x01;
    EFM_ASSERT(i2c_x->IF & 0x01);
    i2c_x->IFC = 0x01;
  } else {
    i2c_x->IFC = 0x01;
    EFM_ASSERT(!(i2c_x->IF & 0x01));
  }

  // Initialize i2c
  I2C_Init(i2c_x, &i2c_open_struct->init);
  // Set GPIO Routes
  // PC11 SDA 16 or SCL 15
  // PC10 SDA 15 or SCL 14
  i2c_x->ROUTELOC0 = i2c_open_struct->SDA_LOC | i2c_open_struct->SCL_LOC;
  i2c_x->ROUTEPEN  = (uint32_t)(((i2c_open_struct->SCL_EN & 0x01) << 1) |
                               (i2c_open_struct->SDA_EN & 0x01));

  // Enable Interrups
  i2c_x->IEN = I2C_IEN_ACK | I2C_IEN_NACK | I2C_IEN_RXDATAV | I2C_IEN_MSTOP;

  // Enable I2C Interrupts with the NVIC
  NVIC_EnableIRQ(I2C0_IRQn);

  _i2c_bus_reset(i2c_x);
}

void _i2c_bus_reset(I2C_TypeDef *i2c_x) {
  // Abort i2c sm actions
  i2c_x->CMD |= I2C_CMD_ABORT;
  // save IEN register
  uint32_t ien_tmp = i2c_x->IEN;
  // Disable interrupts and clear all interrupt flags
  i2c_x->IEN = (uint32_t)0x00;
  i2c_x->IFC = (uint32_t)~0x00;
  // Clear i2c transmit buffer
  i2c_x->CMD = I2C_CMD_CLEARTX;
  // Send start then stop on i2c bus (the message precedence will send them in
  // this order)
  i2c_x->CMD = I2C_CMD_START | I2C_CMD_STOP;
  // This is bad but my thing isn't working...
  //  i2c_x->IFS = I2C_IFS_MSTOP;
  // Wait for stop to finish
  while (!(i2c_x->IF & I2C_IF_MSTOP))
    ;
  // Clear IF's
  i2c_x->IFC = (uint32_t)~0x00;
  // Reset i2c peripheral state
  i2c_x->CMD |= I2C_CMD_ABORT;
  // Enable original interrupts again
  i2c_x->IEN = ien_tmp;
}

// Add a job to the job queue (write)
void i2c_write(I2C_TypeDef *i2c_x, uint8_t device_addr, uint8_t *buff,
               uint32_t len, scheduler_event_t cb_event) {
  // Argument invalid checking
  if (buff == NULL)
    EFM_ASSERT(false);
  volatile i2c_sm_t *sm = _get_sm_from_i2c(i2c_x);

  // Wait for previous i2c_x job to complete
  while (sm->busy)
    ;

  // I2C peripheral must be idle and ready for a new transaction
  if (sm->i2c_x->STATE != I2C_STATE_STATE_IDLE)
    EFM_ASSERT(false);

  sm->busy = true;

  // Construct the job
  sm->job.type        = I2C_JOB_TYPE_WRITE;
  sm->job.buff        = buff;
  sm->job.len         = len;
  sm->job.i           = 0;
  sm->job.device_addr = device_addr << 1;
  sm->job.cb_event    = cb_event;

  // Begin transaction
  _i2c_sm_handle_nack(sm);
}

void i2c_read(I2C_TypeDef *i2c_x, uint8_t device_addr, uint8_t *buff,
              uint32_t len, scheduler_event_t cb_event) {
  // Argument invalid checking
  if (buff == NULL)
    EFM_ASSERT(false);

  volatile i2c_sm_t *sm = _get_sm_from_i2c(i2c_x);

  // Wait for previous i2c_x job to complete
  while (sm->busy)
    ;

  // I2C peripheral must be idle and ready for a new transaction
  if (sm->i2c_x->STATE != I2C_STATE_STATE_IDLE)
    EFM_ASSERT(false);

  sm->busy = true;

  // Construct the job
  sm->job.type        = I2C_JOB_TYPE_READ;
  sm->job.buff        = buff;
  sm->job.len         = len;
  sm->job.i           = 0;
  sm->job.device_addr = device_addr << 1;
  sm->job.cb_event    = cb_event;

  // Begin transaction
  _i2c_sm_handle_nack(sm);
}

// IRQ Handlers
void I2C0_IRQHandler(void) {
  I2C_TypeDef *i2c_x = I2C0;
  // Handle I2C Events
  uint32_t IF;
  // Read interrupt flags
  IF = i2c_x->IF & i2c_x->IEN;
  // Clear interrupt flags
  i2c_x->IFC = IF;

  // I2C_IEN_ACK | I2C_IEN_NACK | I2C_IEN_RXDATAV | I2C_IEN_MSTOP
  volatile i2c_sm_t *sm = _get_sm_from_i2c(i2c_x);
  // I2Cx intependent handler for all IRQ events
  if (IF & I2C_IF_ACK) {
    _i2c_sm_handle_ack(sm);
  }
  if (IF & I2C_IF_NACK) {
    _i2c_sm_handle_nack(sm);
  }
  if (IF & I2C_IF_RXDATAV) {
    _i2c_sm_handle_rxdatav(sm);
  }
  if (IF & I2C_IF_MSTOP) {
    _i2c_sm_handle_mstop(sm);
  }
}

static volatile i2c_sm_t *_get_sm_from_i2c(I2C_TypeDef *i2c_x) {
  if (i2c_x == I2C0) {
    return &i2c0_sm;
  } else if (i2c_x == I2C1) {
    return &i2c1_sm;
  } else {
    EFM_ASSERT(false);
    return 0;
  }
}

static void _i2c_sm_handle_ack(volatile i2c_sm_t *sm) {
  switch (sm->job.type) {
  case I2C_JOB_TYPE_WRITE:
    if (sm->job.i < sm->job.len) {
      // Send next byte
      sm->i2c_x->TXDATA = sm->job.buff[sm->job.i];
      sm->job.i += 1;
    } else {
      // We sent all the data :)
      sm->i2c_x->CMD = I2C_CMD_STOP;
    }
    break;
  case I2C_JOB_TYPE_READ:
    // Do nothing (prepare for RXDATAV)
    break;
  default:
    EFM_ASSERT(false);
    break;
  }
}

static void _i2c_sm_handle_nack(volatile i2c_sm_t *sm) {
  switch (sm->job.type) {
  case I2C_JOB_TYPE_WRITE:
    sm->i2c_x->CMD    = I2C_CMD_START;
    sm->i2c_x->TXDATA = (uint8_t)(sm->job.device_addr | I2C_W);
    break;
  case I2C_JOB_TYPE_READ:
    sm->i2c_x->CMD    = I2C_CMD_START;
    sm->i2c_x->TXDATA = (uint8_t)(sm->job.device_addr | I2C_R);
    break;
  default:
    EFM_ASSERT(false);
    break;
  }
}

static void _i2c_sm_handle_rxdatav(volatile i2c_sm_t *sm) {
  switch (sm->job.type) {
  case I2C_JOB_TYPE_WRITE:
    EFM_ASSERT(false);
    break;
  case I2C_JOB_TYPE_READ:
    EFM_ASSERT(sm->job.i < sm->job.len);
    // Recieve data into rx_buff
    sm->job.buff[sm->job.i] = (uint8_t)sm->i2c_x->RXDATA;
    sm->job.i += 1;
    // Send ACK or NACK
    if (sm->job.i < sm->job.len) {
      sm->i2c_x->CMD = I2C_CMD_ACK;
    } else {
      sm->i2c_x->CMD = I2C_CMD_NACK;
      while (sm->i2c_x->STATUS & I2C_STATUS_PNACK)
        ;
      sm->i2c_x->CMD = I2C_CMD_STOP;
    }
    break;
  default:
    EFM_ASSERT(false);
    break;
  }
}

static void _i2c_sm_handle_mstop(volatile i2c_sm_t *sm) {
  while (sm->i2c_x->STATE != I2C_STATE_STATE_IDLE)
    ;
  sm->busy = false;
  sleep_unblock_mode(I2C_EM_BLOCK);
  // Schedule event
  add_scheduled_event(sm->job.cb_event);
}
