# Serial Driver For STM32

This library provides an Arduino-like `Serial` functionality for STM32 microcontrollers. It is designed to be used with the STM32 Cube IDE and relies on the HAL (Hardware Abstraction Layer) libraries. The implementation is inspired by the Serial API from Arduino, enabling easy migration for developers familiar with the Arduino ecosystem.

## Features
- Implements Arduino-style `Serial` functionality for STM32
- Uses STM32 HAL libraries
- Supports multiple USART instances
- Works with DMA for efficient data reception
- Compatible with `Print`, `Printable`, and `Stream` interfaces

## Requirements
- **STM32 Cube IDE**
- **Configured USART with DMA**: Each USART used in this library must be configured in the graphical configurator to allocate a DMA channel for the USART RX event, with a circular buffer setting.

## Installation
1. Clone this repository into your STM32 Cube IDE project.
2. Ensure that the necessary USART instances are configured correctly in CubeMX.
3. Include the library files in your project and initialize the Serial interface as needed.

## Usage Example

```cpp
#include "Serial.hpp"

Serial usart( &huart1 ); // Assuming huart1 is configured in CubeMX

int main(void) {
    HAL_Init();
    SystemClock_Config();
    usart.begin(115200);
    
    while (1) {
        usart.println( "Hello, STM32!" );
        HAL_Delay( 1000 );
    }
}
```

## License & copyright
This project includes the **Print, Printable, and Stream** libraries from **David A. Mellis** and **Adrian McEwen**.

Serial Driver For STM32 is licensed under the __MIT License__. See `LICENSE` file for details.

