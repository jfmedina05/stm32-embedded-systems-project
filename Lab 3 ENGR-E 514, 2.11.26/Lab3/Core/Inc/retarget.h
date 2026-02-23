/*
 * retarget.h
 *
 *  Created on: Feb 9, 2026
 *      Author: bradyadams
 */

#ifndef RETARGET_H
#define RETARGET_H

#include "stm32l4xx_hal.h"

void RetargetInit(UART_HandleTypeDef *huart);

#endif
