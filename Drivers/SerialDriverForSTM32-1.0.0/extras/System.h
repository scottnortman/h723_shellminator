/*
 * Created on Maj 20 2023
 *
 * Copyright (c) 2023 - Daniel Hajnal
 * hajnal.daniel96@gmail.com
 * This file is part of the Shellminator project.
 * Modified Oct 22 2023
 */

/*
MIT License

Copyright (c) 2023 Daniel Hajnal

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef System_h
#define System_h

#include <ctime>
#include <stdio.h>
#include <stdint.h>
#include <cstdlib>


#if defined(STM32F0xx) || defined(STM32F030x6) || defined(STM32F070xB) || defined(STM32F091xC)
    #include "stm32f0xx_hal.h"
#elif defined(STM32F1xx) || defined(STM32F103xB) || defined(STM32F103xE)
    #include "stm32f1xx_hal.h"
#elif defined(STM32F2xx) || defined(STM32F207xx)
    #include "stm32f2xx_hal.h"
#elif defined(STM32F3xx) || defined(STM32F303xC)
    #include "stm32f3xx_hal.h"
#elif defined(STM32F4xx) || defined(STM32F407xx) || defined(STM32F446xx)
    #include "stm32f4xx_hal.h"
#elif defined(STM32F7xx) || defined(STM32F746xx) || defined(STM32F767xx)
    #include "stm32f7xx_hal.h"
#elif defined(STM32H7xx) || defined(STM32H743xx)
    #include "stm32h7xx_hal.h"
#elif defined(STM32L0xx) || defined(STM32L053xx)
    #include "stm32l0xx_hal.h"
#elif defined(STM32L1xx) || defined(STM32L152xE)
    #include "stm32l1xx_hal.h"
#elif defined(STM32L4xx) || defined(STM32L476xx)
    #include "stm32l4xx_hal.h"
#else
    #error "Unsupported STM32 family! Check your compiler settings."
#endif

unsigned long millis();
void delay( uint32_t x );

#endif
