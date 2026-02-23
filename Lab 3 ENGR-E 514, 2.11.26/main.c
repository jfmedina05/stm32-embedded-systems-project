/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <retarget.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stm32l4xx_ll_usart.h>
/* USER CODE END Includes */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define UART_RX_BUFFER_SIZE     128U
#define CLI_LINE_BUFFER_SIZE    128U
/* USER CODE END PD */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
static volatile uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
static volatile uint16_t uartRxHead;
static volatile uint16_t uartRxTail;

static uint8_t cliLineBuffer[CLI_LINE_BUFFER_SIZE];
static uint16_t cliLineIndex;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

/* USER CODE BEGIN PFP */
typedef struct command
{
    const char *cmd_string;
    void (*cmd_function)(char *arg);
} command_t;

void USART2_RxIRQHandler(void);

static int uart_rx_pop(uint8_t *ch);
static int parse_command(uint8_t *line, uint8_t **command, uint8_t **args);
static int execute_command(uint8_t *line);
static void print_prompt(void);

static void help_command(char *arguments);
static void lof_command(char *arguments);
static void lon_command(char *arguments);
static void test_command(char *arguments);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */
static command_t commands[] =
{
    {"help", help_command},
    {"lof",  lof_command},
    {"lon",  lon_command},
    {"test", test_command},
    {0, 0}
};
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  */
int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_USART2_UART_Init();

    /* USER CODE BEGIN 2 */
    RetargetInit(&huart2);
    printf("System Up and Running\r\n");
    print_prompt();
    /* USER CODE END 2 */

    while (1)
    {
        uint8_t ch;

        if (uart_rx_pop(&ch) == 1)
        {
            if ((ch == '\r') || (ch == '\n'))
            {
                printf("\r\n");

                cliLineBuffer[cliLineIndex] = '\0';

                if (cliLineIndex > 0U)
                {
                    if (execute_command(cliLineBuffer) == 0)
                    {
                        printf("OK\r\n");
                    }
                    else
                    {
                        printf("NOK\r\n");
                    }
                }

                cliLineIndex = 0U;
                print_prompt();
            }
            else if ((ch == '\b') || (ch == 0x7FU))
            {
                if (cliLineIndex > 0U)
                {
                    cliLineIndex--;
                    printf("\b \b");
                }
            }
            else if (isprint((int)ch))
            {
                if (cliLineIndex < (CLI_LINE_BUFFER_SIZE - 1U))
                {
                    cliLineBuffer[cliLineIndex++] = ch;
                    printf("%c", ch);
                }
            }
        }
        else
        {
            HAL_Delay(1);
        }
    }
}
