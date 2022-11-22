// /*
//  * File: queue.c
//  * Author: Matthew Teta
//  * Project: CSCI 3753 Programming Assignment 3
//  * Create Date: 07/10/2022
//  *
//  */

// #include "queue.h"

// queue_t queue_new(int capacity)
// {
//     if (capacity <= 0)
//         EFM_ASSERT(0);
//     // Allocate space for data
//     void **data = (void **)malloc((size_t)(capacity * sizeof(void *)));

//     queue_t arr = queue_init_from_buffer(capacity, 0, data);

//     // Indicate that memory was allocated internally (for cleanup purposes)
//     arr.mem_managed = 1;

//     return arr;
// }

// queue_t queue_init_from_buffer(const int capacity, int index, void **data)
// {
//     if (data == NULL || capacity <= 0)
//         EFM_ASSERT(0);
//     queue_t arr;
//     arr.capacity = capacity;
//     arr.data = data;
//     arr.index = index;
//     arr.mem_managed = 0;

//     return arr;
// }

// int queue_push(queue_t *arr, void *val)
// {
//     if (arr == NULL)
//         return -1;

//     if (arr->index < 0 || arr->index >= arr->capacity)
//     {
//         return -2;
//     }
//     // Actually do push
//     arr->data[arr->index] = val;
//     arr->index += 1;

//     return 0;
// }

// int queue_pop(queue_t *arr, void **val)
// {

//     // Actually do pop
//     arr->index -= 1;
//     if (arr->index < 0 || arr->index >= arr->capacity)
//     {
//         return -2;
//     }
//     else
//     {
//         *val = arr->data[arr->index];
//     }

//     return 0;
// }

// int queue_getcapacity(queue_t *arr)
// {
//     if (arr == NULL)
//         return -1;
//     return arr->capacity;
// }

// int queue_isempty(queue_t *arr)
// {
//     if (arr == NULL)
//         return -1;
//     int items_avail;
//     items_avail = arr->index;
//     return items_avail == 0;
// }

// int queue_isfull(queue_t *arr)
// {
//     if (arr == NULL)
//         return -1;
//     int items_avail;
//     items_avail = arr->index;
//     return items_avail == arr->capacity;
// }

// void queue_delete(queue_t *arr)
// {
//     if (arr == NULL)
//         return;
//     if (arr->mem_managed)
//         free(arr->data);
// }