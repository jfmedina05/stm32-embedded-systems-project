/*
 * command.c
 *
 *  Created on: Feb 9, 2026
 *      Author: bradyadams
 */

#include "command.h"
#include <stdio.h>
#include <string.h>
#include "main.h"

void help_command(char *args);
void lon_command(char *args);
void lof_command(char *args);
void test_command(char *args);

command_t commands[] = {
    {"help", help_command},
    {"lon",  lon_command},
    {"lof",  lof_command},
    {"test", test_command},
    {0, 0}
};

static int parse_command(uint8_t *line, uint8_t **cmd, uint8_t **args)
{
    *cmd = line;
    *args = NULL;

    for (int i = 0; line[i]; i++) {
        if (line[i] == ',') {
            line[i] = 0;
            *args = &line[i + 1];
            break;
        }
    }
    return 0;
}

int execute_command(uint8_t *line)
{
    uint8_t *cmd;
    uint8_t *args;
    command_t *p = commands;

    if (!line) return -1;

    parse_command(line, &cmd, &args);

    while (p->cmd_string) {
        if (!strcmp(p->cmd_string, (char *)cmd)) {
            p->cmd_function((char *)args);
            return 0;
        }
        p++;
    }

    printf("NOK\n\r");
    return -1;
}


void help_command(char *args)
{
    printf("Available Commands:\n\r");
    printf("help\n\r");
    printf("lon\n\r");
    printf("lof\n\r");
    printf("test\n\r");
}

void lon_command(char *args)
{
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_SET);
}

void lof_command(char *args)
{
    HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
}

void test_command(char *args)
{
    char *token;

    if (!args) return;

    token = strtok(args, ",");
    while (token) {
        printf("%s\n\r", token);
        token = strtok(NULL, ",");
    }
}
