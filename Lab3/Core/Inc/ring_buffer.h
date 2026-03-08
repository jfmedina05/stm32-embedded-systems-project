#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

#define RB_SIZE 128

typedef struct {
    uint8_t buffer[RB_SIZE];
    volatile uint16_t head;
    volatile uint16_t tail;
} ring_buffer_t;

void rb_init(ring_buffer_t *rb);
int rb_put(ring_buffer_t *rb, uint8_t data);
int rb_get(ring_buffer_t *rb, uint8_t *data);

#endif
