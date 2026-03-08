/*
 * ring_buffer.c
 *
 *  Created on: Feb 9, 2026
 *      Author: bradyadams
 */
#include "ring_buffer.h"

void rb_init(ring_buffer_t *rb)
{
    rb->head = 0;
    rb->tail = 0;
}

int rb_put(ring_buffer_t *rb, uint8_t data)
{
    uint16_t next = (rb->head + 1) % RB_SIZE;
    if (next == rb->tail) return -1; // buffer full
    rb->buffer[rb->head] = data;
    rb->head = next;
    return 0;
}

int rb_get(ring_buffer_t *rb, uint8_t *data)
{
    if (rb->head == rb->tail) return -1; // empty
    *data = rb->buffer[rb->tail];
    rb->tail = (rb->tail + 1) % RB_SIZE;
    return 0;
}
