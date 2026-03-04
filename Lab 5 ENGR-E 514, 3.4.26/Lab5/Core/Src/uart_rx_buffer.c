/*
 * uart_rx_buffer.c
 *
 *  Created on: Feb 11, 2026
 *      Author: mavwdj
 */


#include "uart_rx_buffer.h"

void rxbuf_init(circ_buf_t *cb) {
  cb->head = 0;
  cb->tail = 0;
}

static uint16_t next_index(uint16_t i) {
  return (uint16_t)((i + 1) % RX_BUF_SIZE);
}

int rxbuf_push(circ_buf_t *cb, uint8_t ch) {
  uint16_t next = next_index(cb->head);
  if (next == cb->tail) {
    return -1; // full
  }
  cb->buf[cb->head] = ch;
  cb->head = next;
  return 0;
}

int rxbuf_pop(circ_buf_t *cb, uint8_t *ch) {
  if (cb->head == cb->tail) {
    return -1; // empty
  }
  *ch = cb->buf[cb->tail];
  cb->tail = next_index(cb->tail);
  return 0;
}

int rxbuf_available(circ_buf_t *cb) {
  if (cb->head >= cb->tail) return (cb->head - cb->tail);
  return (RX_BUF_SIZE - cb->tail + cb->head);
}
