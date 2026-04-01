/*
 * uart_rx_buffer.h
 *
 *  Created on: Mar 25, 2026
 *      Author: bradyadams
 */

#ifndef UART_RX_BUFFER_H
#define UART_RX_BUFFER_H

#include <stdint.h>

#define RXBUF_SIZE 128

typedef struct {
    uint8_t buffer[RXBUF_SIZE];
    uint16_t head;
    uint16_t tail;
} circ_buf_t;

void rxbuf_init(circ_buf_t *buf);
int rxbuf_push(circ_buf_t *buf, uint8_t data);
int rxbuf_pop(circ_buf_t *buf, uint8_t *data);

#endif
