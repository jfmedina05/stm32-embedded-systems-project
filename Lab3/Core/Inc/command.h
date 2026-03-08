/*
 * command.h
 *
 *  Created on: Feb 9, 2026
 *      Author: bradyadams
 */

#ifndef COMMAND_H
#define COMMAND_H

#include <stdint.h>

typedef struct {
    char *cmd_string;
    void (*cmd_function)(char *args);
} command_t;

int execute_command(uint8_t *line);

#endif
