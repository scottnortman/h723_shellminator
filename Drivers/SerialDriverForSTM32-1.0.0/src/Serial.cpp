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

#include "Serial.hpp"

const char* Serial::dmaErrorMessage = "DMA Error!\r\n";

Serial::Serial( UART_HandleTypeDef *usart_device_p ){
	usart_device = usart_device_p;
}

bool Serial::begin( uint32_t baudrate_p ){

	if( usart_device == NULL ){
		return false;
	}

	if (HAL_UART_DeInit( usart_device ) != HAL_OK){
		return false;
	}

	usart_device -> Init.BaudRate = baudrate_p;


	if (HAL_UART_Init( usart_device ) != HAL_OK){
		return false;
	}

	receive_buffer_counter = 0;

	if( usart_device -> hdmarx == NULL ){
		return false;
	}

	if (HAL_UART_Receive_DMA( usart_device, receive_buffer, SERIAL_RECEIVE_BUFFER_LENGTH ) != HAL_OK){
		return false;
	}
	return true;
}

int Serial::available(){

	uint32_t dma_ptr;

	DMA_Stream_TypeDef * dma_str;
	void * Instance;

	if( usart_device == NULL ){
		return 0;
	}

	if( usart_device -> hdmarx == NULL ){
		return 0;
	}

	//dma_ptr = SERIAL_RECEIVE_BUFFER_LENGTH - ( usart_device -> hdmarx -> Instance -> NDTR );
	Instance = usart_device -> hdmarx -> Instance;
	dma_str = (DMA_Stream_TypeDef *)Instance;
	dma_ptr = SERIAL_RECEIVE_BUFFER_LENGTH - (dma_str -> NDTR );


	if( dma_ptr == receive_buffer_counter ){
		return 0;
	}

	if( dma_ptr > receive_buffer_counter ){
		return dma_ptr - receive_buffer_counter;
	}

	return SERIAL_RECEIVE_BUFFER_LENGTH - receive_buffer_counter +  dma_ptr;

}

int Serial::read(){

	uint32_t dma_ptr;
	uint8_t ret;

	DMA_Stream_TypeDef * dma_str;
	void * Instance;

	if( usart_device == NULL ){
		return -1;
	}

	if( usart_device -> hdmarx == NULL ){
		return -1;
	}

	//dma_ptr = SERIAL_RECEIVE_BUFFER_LENGTH - ( usart_device -> hdmarx -> Instance -> NDTR );
	Instance = usart_device -> hdmarx -> Instance;
	dma_str = (DMA_Stream_TypeDef *)Instance;
	dma_ptr = SERIAL_RECEIVE_BUFFER_LENGTH - (dma_str -> NDTR );


	if( dma_ptr == receive_buffer_counter ){
		return -1;
	}


	ret = (uint8_t)receive_buffer[ receive_buffer_counter ];
	receive_buffer_counter++;

	if( receive_buffer_counter >= SERIAL_RECEIVE_BUFFER_LENGTH ){
		receive_buffer_counter = 0;
	}

	return ret;

}

int Serial::peek(){

	uint32_t dma_ptr;
	DMA_Stream_TypeDef * dma_str;
	void * Instance;

	if( usart_device == NULL ){
		return -1;
	}

	if( usart_device -> hdmarx == NULL ){
		return -1;
	}

	//dma_ptr = SERIAL_RECEIVE_BUFFER_LENGTH - ( usart_device -> hdmarx -> Instance -> NDTR );
	Instance = usart_device -> hdmarx -> Instance;
	dma_str = (DMA_Stream_TypeDef *)Instance;
	dma_ptr = SERIAL_RECEIVE_BUFFER_LENGTH - (dma_str -> NDTR );

	if( dma_ptr == receive_buffer_counter ){
		return -1;
	}

	return (uint8_t)receive_buffer[ receive_buffer_counter ];

}

void Serial::flush(){

}

size_t Serial::write( uint8_t b ){

	if( usart_device == NULL ){
		return 0;
	}

	if( usart_device -> hdmarx == NULL ){
		HAL_UART_Transmit( usart_device, (const uint8_t*)dmaErrorMessage, strlen( dmaErrorMessage ), 500 );
		return 0;
	}

	if( HAL_UART_Transmit( usart_device, &b, 1, SERIAL_TIMEOUT ) == HAL_OK ){
		return 1;
	}

	return 0;
}

size_t Serial::write( const uint8_t *buff, size_t size ){

	if( usart_device == NULL ){
		return 0;
	}

	if( usart_device -> hdmarx == NULL ){
		HAL_UART_Transmit( usart_device, (const uint8_t*)dmaErrorMessage, strlen( dmaErrorMessage ), 500 );
		return 0;
	}

	if( HAL_UART_Transmit( usart_device, buff, size, SERIAL_TIMEOUT ) == HAL_OK ){
		return 1;
	}

	return 0;
}

size_t Serial::write( const char *str ){

	int size;

	if( usart_device == NULL ){
		return 0;
	}

	if( usart_device -> hdmarx == NULL ){
		HAL_UART_Transmit( usart_device, (const uint8_t*)dmaErrorMessage, strlen( dmaErrorMessage ), 500 );
		return 0;
	}

	size = strlen( str );

	if( HAL_UART_Transmit( usart_device, (const uint8_t*)str, size, SERIAL_TIMEOUT ) == HAL_OK ){
		return 1;
	}

	return 0;
}

