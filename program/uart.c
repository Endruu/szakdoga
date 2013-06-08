#include "global.h"

#define MAX_SEND_TRIES	500

uint _echo = 0;
int _buffer_index = 0;

uint toggleEcho() {
	return _echo = !_echo;
}

#ifdef _COMPILE_WITH_BLACKFIN

#include <blackfin.h>

uint sendChar(char c) {
	int i = 0;
	while(!(*pUART0_LSR & 0x0020) ) {				// while transmit is pending
		if( i++ > MAX_SEND_TRIES ) {					// if could not send in MAX_SEND_TRIES
			error(0);														// returns with error
		}
	}
	*pUART0_THR = c;
	return 1;
}

uint receiveString() {
	char in = *pUART0_RBR;
	
	if( uartRequest ) {								// buffer is in use
		if( _echo ) sendChar('~');
		error(0);
	}
	
	if( _buffer_index < UART_BUF_SIZE ) {
		if( in == 13 ) {							// ENTER
			if( _echo ) sendChar('\n');
			uart_buffer[_buffer_index] = '\0';
			_buffer_index = 0;
			uartRequest = 1;
			if( _echo ) *pUART0_THR = 13;
		}
		else if( in == 8 ) {						// backspace
			if( _echo ) sendChar('\b');
			if( _buffer_index ) _buffer_index--;
			if( _echo ) sendChar(' ');
			if( _echo ) sendChar('\b');
		}
		else {
			uart_buffer[_buffer_index++] = in;
			if( _echo ) *pUART0_THR = in;
		}
		
	}
	else {
		if( in == 8 ) {									// backspace
			if( _echo ) *pUART0_THR = in;
			if( _buffer_index ) _buffer_index--;
			if( _echo ) sendChar(' ');
			if( _echo ) sendChar('\b');
		} else {
			if( _echo ) sendChar('~');
			_buffer_index++;
			if( in == 13 ) {							// ENTER
				if( _echo ) sendChar('\n');
				uart_buffer[_buffer_index-1] = '\0';
				_buffer_index = 0;
				if( _echo ) *pUART0_THR = in;
				error(0);								// the buffer is full
			}
		}
	}
	return _buffer_index;
}

#endif
