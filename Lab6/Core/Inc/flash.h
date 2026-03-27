/*
 * flash.h
 *
 *  Created on: Mar 25, 2026
 *      Author: bradyadams
 */

#ifndef FLASH_H
#define FLASH_H

#include "stm32l4xx_hal.h"

#define FLASH_START_ADDR  0x0801F800

void flash_write_record(uint32_t timestamp);
void flash_read_all(void);
void flash_erase(void);

#endif
