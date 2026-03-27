/*
 * commands.c
 *
 *  Created on: Mar 25, 2026
 *      Author: bradyadams
 */

#include "commands.h"
#include "flash.h"
#include "stm32l4xx_hal.h"
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim2;

void cmd_sample(void)
{
    uint32_t time = __HAL_TIM_GET_COUNTER(&htim2);
    flash_write_record(time);
}

void cmd_log(void)
{
    flash_read_all();
}

void cmd_data(void)
{
    flash_read_all();
}

void cmd_erase(void)
{
    flash_erase();
}
