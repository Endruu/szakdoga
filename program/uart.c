#include "global.h"

#ifdef _COMPILE_WITH_BLACKFIN

#include <blackfin.h>

uint _echo = 0;
int _buffer_index = 0;

uint switchEcho() {
	return _echo = !_echo;
}

uint receiveString() {
	char in = *pUART0_RBR;
	
	if( uartRequest ) {								// buffer is in use
		if( _echo ) *pUART0_THR = '~';
		error(0);
	}
	
	if( _buffer_index < UART_BUF_SIZE ) {
		if( in == 13 ) {							// ENTER
			if( _echo ) *pUART0_THR = '\n';
			uart_buffer[_buffer_index] = '\0';
			_buffer_index = 0;
			uartRequest = 1;
		} else {
			uart_buffer[_buffer_index++] = in;
		}
		if( _echo ) *pUART0_THR = in;
	}
	else {
		if( _buffer_index == UART_BUF_SIZE ) {
			if( _echo ) *pUART0_THR = '~';
			_buffer_index++;
			if( in == 13 ) {							// ENTER
				if( _echo ) *pUART0_THR = '\n';
				uart_buffer[_buffer_index-1] = '\0';
				_buffer_index = 0;
			}
			if( _echo ) *pUART0_THR = in;
			error(0);									// the buffer is full
		} else {
			if( in == 13 ) {							// ENTER
				if( _echo ) *pUART0_THR = '\n';
				_buffer_index = 0;
			}
			_buffer_index++;
		}
	}
	return _buffer_index;
}

#endif
