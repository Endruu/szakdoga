#include "global.h"

uint _echo = 0;
int _buffer_index = 0;

uint toggleEcho() {
	return _echo = !_echo;
}

#ifdef _COMPILE_WITH_BLACKFIN

#include <blackfin.h>

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
		} else if( in == 8 ) {						// backspace
			if( _echo ) *pUART0_THR = in;
			if( _buffer_index ) _buffer_index--;
		} else {
			uart_buffer[_buffer_index++] = in;
		}
		if( _echo ) *pUART0_THR = in;
	}
	else {
		if( in == 8 ) {									// backspace
			if( _echo ) *pUART0_THR = in;
			if( _buffer_index ) _buffer_index--;
		} else {
			if( _echo ) *pUART0_THR = '~';
			_buffer_index++;
			if( in == 13 ) {							// ENTER
				if( _echo ) *pUART0_THR = '\n';
				uart_buffer[_buffer_index-1] = '\0';
				_buffer_index = 0;
				error(0);								// the buffer is full
			}
		}
	}
	return _buffer_index;
}

#endif
