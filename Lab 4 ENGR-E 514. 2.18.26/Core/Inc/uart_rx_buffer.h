/*
 * uart_rx_buffer.h
 *
 *  Created on: Feb 11, 2026
 *      Author: mavwdj
 */


#pragma once
#include <stdint.h>

#define RX_BUF_SIZE 256

typedef struct {
  volatile uint8_t buf[RX_BUF_SIZE];
  volatile uint16_t head; // write index
  volatile uint16_t tail; // read index
} circ_buf_t;

void rxbuf_init(circ_buf_t *cb);
int  rxbuf_push(circ_buf_t *cb, uint8_t ch);  // returns 0 ok, -1 full
int  rxbuf_pop (circ_buf_t *cb, uint8_t *ch); // returns 0 ok, -1 empty
int  rxbuf_available(circ_buf_t *cb);
