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

#ifndef SERIALDRIVERFORSTM32_SERIAL_HPP_
#define SERIALDRIVERFORSTM32_SERIAL_HPP_

#include <stdint.h>
#include <string.h>
#include "Stream.h"
#include "System.h"

/// Length of the receive buffer
///
/// You can specify how much bytes you want to recive maximum.
/// If this buffer is to large, then it will waste your RAM.
/// If this buffer is to short, then you can't send your data properly.
#define SERIAL_RECEIVE_BUFFER_LENGTH 100

#define SERIAL_TIMEOUT	1000

class Serial : public Stream{

public:

	/// Constructor
	///
	/// @param usart_device_p pointer to your usart handler.
	/// Example:
	/// `Serial serial( &huart3 )`
	Serial( UART_HandleTypeDef *usart_device_p );

	/// Begin function
	///
	/// It initalises the peripherial and starts the reception with DMA.
	/// @param baudrate_p You can specify the baudrate with this argument.
	bool begin( uint32_t baudrate_p );

	/// Available bytes in the channel.
	///
	/// @returns The available bytes in the channel.
	int    available() override;

	/// Read one byte form the channel.
	///
	/// @returns Read and return one byte form the channel. The byte will be removed from the channel.
	int    read() override;

	/// Peek the firtst byte from the channel.
	///
	/// @returns Read and return one byte form the channel. The byte will NOT be removed from the channel.
	int    peek() override;

	/// Flush the channel.
	void   flush() override;

	/// Write one byte to the channel.
	///
	/// @param b The value that has to be written to the channel.
	/// @returns The number of bytes that has been sucessfully written to the channel. Because it is the base class, it returns 0.
	size_t write( uint8_t b ) override;

	/// Write byte array to the channel.
	///
	/// @param buff The array that has to be written to the channel.
	/// @param size The size of the array.
	/// @returns The number of bytes that has been sucessfully written to the channel. Because it is the base class, it returns 0.
	size_t write( const uint8_t *buff, size_t size ) override;

	/// Write a C string to the channel.
	///
	/// @param str The C string that has to be written to the channel.
	/// @returns The number of bytes that has been sucessfully written to the channel. Because it is the base class, it returns 0.
	size_t write( const char *str ) override;



private:
	/// Uart peripherial address
	UART_HandleTypeDef *usart_device = NULL;

	/// Baudrate setting
	uint32_t baudrate = 0;

	/// Recive buffer
	uint8_t receive_buffer[ SERIAL_RECEIVE_BUFFER_LENGTH ];

	/// Points to the next free element in the recive buffer
	uint32_t receive_buffer_counter = 0;

	/// Default error message in case of wrong DMA configuration.
	static const char* dmaErrorMessage;

};

#endif /* SERIALDRIVERFORSTM32_SERIAL_HPP_ */
