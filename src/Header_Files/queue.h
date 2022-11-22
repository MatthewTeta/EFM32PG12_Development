// #ifndef QUEUE_HG
// #define	QUEUE_HG

// #include <stddef.h>
// #include "em_assert.h"
// #include "em_core.h"

// typedef struct queue_t {
//     int capacity;
//     int index;
//     void **data;
//     // Internal indicator for if memory was allocated in constructor
//     int mem_managed;
// } queue_t;

// // Initialize queue and malloc memory
// queue_t queue_new(const int capacity);

// // Initialize queue from existing buffer
// queue_t queue_init_from_buffer(const int capacity, int index, void **data);

// // Insert item onto stack
// int     queue_push(queue_t *arr, void *val);

// // Pop from stack (block indefinitely)
// int     queue_pop(queue_t *arr, void **val);

// // Get buffer size
// int     queue_getcapacity(queue_t *arr);

// // return 1 if empty
// int     queue_isempty(queue_t *arr);

// // return 1 if full
// int     queue_isfull(queue_t *arr);

// // cleanup queue (free memory only if initialized with queue_new)
// void    queue_delete(queue_t *arr);

// #endif
