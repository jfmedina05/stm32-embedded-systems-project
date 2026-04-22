/*
 * retarget.c
 *
 *  Created on: Mar 25, 2026
 *      Author: bradyadams
 */

#include "retarget.h"
#include <stdio.h>

UART_HandleTypeDef *g_huart;

void RetargetInit(UART_HandleTypeDef *huart)
{
    g_huart = huart;
    setvbuf(stdout, NULL, _IONBF, 0);
}

// redirect printf → UART
int __io_putchar(int ch)
{
    HAL_UART_Transmit(g_huart, (uint8_t *)&ch, 1, HAL_MAX_DELAY);
    return ch;
}
