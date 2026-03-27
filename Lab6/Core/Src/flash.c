/*
 * flash.c
 *
 *  Created on: Mar 25, 2026
 *      Author: bradyadams
 */

#include "flash.h"
#include <string.h>
#include <stdio.h>

extern UART_HandleTypeDef huart2;

static uint32_t current_address = FLASH_START_ADDR;

void flash_erase(void)
{
    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef erase;
    uint32_t page_error;

    erase.TypeErase = FLASH_TYPEERASE_PAGES;
    erase.Page = (FLASH_START_ADDR - FLASH_BASE) / FLASH_PAGE_SIZE;
    erase.NbPages = 1;

    HAL_FLASHEx_Erase(&erase, &page_error);

    HAL_FLASH_Lock();

    current_address = FLASH_START_ADDR;

    char msg[] = "Flash Erased\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}

void flash_write_record(uint32_t timestamp)
{
    HAL_FLASH_Unlock();

    uint64_t data = timestamp;

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, current_address, data);

    HAL_FLASH_Lock();

    current_address += 8;

    char msg[] = "Sample Stored\r\n";
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, strlen(msg), 100);
}

void flash_read_all(void)
{
    uint32_t addr = FLASH_START_ADDR;
    char buffer[64];

    while (addr < current_address)
    {
        uint64_t raw = *(uint64_t*)addr;
        uint32_t data = (uint32_t)raw;

        sprintf(buffer, "Time: %lu\r\n", data);
        HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

        addr += 8;  // match write size
    }
}
