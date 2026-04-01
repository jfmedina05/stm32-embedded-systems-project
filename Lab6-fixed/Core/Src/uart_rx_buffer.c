/*
 * uart_rx_buffer.c
 *
 *  Created on: Mar 25, 2026
 *      Author: bradyadams
 */

#include "uart_rx_buffer.h"

void rxbuf_init(circ_buf_t *buf)
{
    buf->head = 0;
    buf->tail = 0;
}

int rxbuf_push(circ_buf_t *buf, uint8_t data)
{
    uint16_t next = (buf->head + 1) % RXBUF_SIZE;

    // buffer full
    if (next == buf->tail)
        return -1;

    buf->buffer[buf->head] = data;
    buf->head = next;

    return 0;
}

int rxbuf_pop(circ_buf_t *buf, uint8_t *data)
{
    // buffer empty
    if (buf->head == buf->tail)
        return -1;

    *data = buf->buffer[buf->tail];
    buf->tail = (buf->tail + 1) % RXBUF_SIZE;

    return 0;
}
