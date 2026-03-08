/*
 * retarget.c
 *
 *  Created on: Feb 9, 2026
 *      Author: bradyadams
 */

#include "retarget.h"
#include <stdio.h>

static UART_HandleTypeDef *uart_handle;

void RetargetInit(UART_HandleTypeDef *huart)
{
    uart_handle = huart;
}

/* Redirect printf() */
int _write(int file, char *ptr, int len)
{
    HAL_UART_Transmit(uart_handle, (uint8_t *)ptr, len, HAL_MAX_DELAY);
    return len;
}

/* Redirect getchar() */
int _read(int file, char *ptr, int len)
{
    HAL_UART_Receive(uart_handle, (uint8_t *)ptr, 1, HAL_MAX_DELAY);
    return 1;
}
