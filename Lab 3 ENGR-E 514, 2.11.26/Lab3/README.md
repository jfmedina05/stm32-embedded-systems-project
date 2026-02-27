# Lab 3 – UART Command Line Interface (CLI)

## Summary
In Lab 3, I implemented a UART-based Command Line Interface (CLI) on the STM32 (USART2 @ 115200 baud) to support interactive command entry through a serial terminal. The design uses an interrupt-driven receive path with a circular RX buffer, a line editor (supports backspace), and a command dispatcher that parses comma-delimited commands and executes matching handlers. After each command, the CLI prints either `OK` or `NOK` and re-displays the prompt.

---

## Implemented Features
- USART2 configured for serial I/O
- `printf()` redirected to UART using `RetargetInit(&huart2)`
- Interrupt-driven UART RX (RXNE) feeding a software circular buffer
- CLI line buffering with:
  - Enter key handling (`\r` or `\n`) to submit a command
  - Backspace handling (`\b` or `0x7F`) to edit input
  - Printable-character filtering using `isprint()`
- Comma-delimited command parsing
- Command dispatch table (`command_t`) mapping command strings to handlers
- Standardized response messages:
  - `OK` on successful command execution
  - `NOK` on invalid/unknown command
- Prompt printing after every command

---

## Command Table
Commands are stored in a dispatch table:

```c
typedef struct command
{
    const char *cmd_string;
    void (*cmd_function)(char *arg);
} command_t;

static command_t commands[] =
{
    {"help", help_command},
    {"lof",  lof_command},
    {"lon",  lon_command},
    {"test", test_command},
    {0, 0}
};
```

---

## Supported Commands

| Command | Description | Example |
|--------|-------------|---------|
| `help` | Prints available commands | `help` |
| `lon`  | Turns LED ON | `lon` |
| `lof`  | Turns LED OFF | `lof` |
| `test` | Debug/argument parsing test command | `test,abc,123` |

---

## CLI Behavior

### Startup
On boot the system prints a startup message and the CLI prompt:

```
System Up and Running
IULS>
```

### Command Entry
- Characters typed in the terminal are echoed back.
- Backspace removes the last typed character.
- Pressing Enter submits the command line.

After processing a command, the system prints:

- `OK` if the command was recognized and executed
- `NOK` if the command was not recognized or failed

Then the prompt is printed again.

Example session:

```
IULS> help
OK
IULS> lon
OK
IULS> lof
OK
IULS> badcommand
NOK
IULS>
```

---

## UART RX Architecture

### Circular RX Buffer
Incoming bytes received over USART2 are stored in a circular buffer:

- `uartRxBuffer[UART_RX_BUFFER_SIZE]`
- `uartRxHead` / `uartRxTail` indices

The main loop pulls bytes from the buffer using:

```c
static int uart_rx_pop(uint8_t *ch);
```

### Main Loop Flow
```
USART2 RX Interrupt
        ↓
Circular RX Buffer
        ↓
uart_rx_pop()
        ↓
CLI Line Buffer
        ↓
parse_command()
        ↓
execute_command()
        ↓
OK / NOK + prompt
```

---

## File Notes
- `main.c` contains:
  - UART RX buffer + CLI line buffer
  - command parsing + execution logic
  - prompt printing
  - command handlers (help/lon/lof/test)
- `RetargetInit()` is used to redirect `printf()` to USART2

---

## Hardware / Tools
- STM32 Nucleo board (USART2)
- Serial terminal (PuTTY, TeraTerm, CoolTerm, etc.)
- USART2 settings:
  - Baud: `115200`
  - Data bits: `8`
  - Parity: `None`
  - Stop bits: `1`

---

## Demonstrated Functionality
- UART serial terminal interaction
- Reliable command entry + line editing (enter + backspace)
- Command parsing and dispatching
- LED control from the CLI
- Consistent `OK` / `NOK` responses and prompt regeneration

---