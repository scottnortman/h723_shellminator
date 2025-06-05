# h723_shellminator

**Lightweight Shell Emulator for STM32H7 (based on Shellminator)**

## Overview
This repository contains the source code for the `h723_shellminator` project—a small interactive shell interface running on an STM32H723ZG MCU, based on the Shellminator framework.  
Users can type commands over a UART console, parse arguments, and execute functions such as LED control, sensor reads, etc.

## Features
- ANSI‐style command parsing (split arguments, sanitize input)  
- Built‐in commands: 
  - `led <0|1|2> <0|1>` (toggle onboard LEDs)  
  - Custom commands can be registered via function pointers in `shell_cmds.h`.
- Uses STM32CubeH7 HAL for peripheral init (UART, GPIO, SysTick).

## Prerequisites
- **Hardware**: NUCLEO-H723ZG board (STM32H7) connected to PC via ST-Link USB  
- **Toolchain**: 
  - STM32CubeIDE (v1.18.1 or later)  
  - arm-none-eabi-gcc (bundled with CubeIDE)  
- **Libraries**: STM32CubeH7 HAL drivers (included in this project)

## Getting Started

1. **Open the project in STM32CubeIDE**  
   - Launch CubeIDE → _File → Open Projects from File System…_  
   - Browse to `…/h723_shellminator` and finish.  
2. **Configure your UART pins** (if needed)  
   - By default, this repo assumes `USART3` on `PD8` (TX) and `PD9` (RX).  
   - Adjust `MX_USART3_UART_Init()` in `main.c` if your wiring differs.  
3. **Build & Flash**  
   - Click the “hammer” icon (Build) in CubeIDE.  
   - Once the build succeeds, click the “debug” or “run” icon to flash the NUCLEO board.
4. **Open a Serial Console**  
   - Use a USB-to-UART tool (e.g. PuTTY or the built-in ST-Link Virtual COM)  
   - Baud rate: **115200**, 8-N-1, no flow control.  
5. **Interact with the Shell**  
   - Type `help` and press Enter to see available commands.  
   - Example: `led 0 1` → turns on LED0; `led 2 0` → turns off LED2.

## 

Note the following issues were encountered:
1. When following the instructions from  https://github.com/dani007200964/Shellminator and the related project https://commanderapi.org/html/index.html I was unable to build; see the bug report:  https://github.com/dani007200964/Shellminator/issues/28

2. I made the source code updates per the above build issues

3. Both build symbols need to be defined under C/C++ Build/Paths and Symbols/#Symbols
a. STM32H723xx
b. STM32H7xx

