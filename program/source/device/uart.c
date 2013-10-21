#include "../../headers/device/uart.h"

#ifdef _COMPILE_WITH_BLACKFIN

#include <blackfin.h>

uint _ticks_per_char = 1;
int _echo = ECHO_DEFAULT;
int _buffer_index = 0;

uint toggleEcho() {

	sendChar('E');
	sendChar('C');
	sendChar('H');
	sendChar('O');
	sendChar(' ');
	sendChar('O');
	
	_echo = !_echo;
	
	if( _echo ) {
		sendChar('N');
	} else {
		sendChar('F');
		sendChar('F');
	}
	
	sendChar(10);								// Line Feed
	sendChar(13);								// Carriage Return
	
	return _echo;
	
}

uint sendChar(char c) {
	int i = 0, j = 0;
	while(!(*pUART0_LSR & 0x0020) ) {			// while transmit is pending
		if( j++ > _ticks_per_char ) {			// prev send should be finished in _ticks_per_char clock cycle
			j = 0;
			if( i++ > MAX_SEND_TRIES ) {		// if could not send in MAX_SEND_TRIES
				return 0;						// returns with error
			}
		}
	}
	*pUART0_THR = c;
	return 1;
}

void receiveString() {
	char in = *pUART0_RBR;
	
	if( _buffer_index == 0 && in == ECHO_TOGGLE_CHAR ) {
		toggleEcho();
	}
	
	if( pendingCommand == 1 ) {							// buffer is in use
		sendChar('!');
		if( _echo ) {
			sendChar(10);								// Line Feed
			sendChar(13);								// Carriage Return
		}
		return;
	}
	
	if( _buffer_index < INPUT_BUF_SIZE ) {
		if( in == 13 ) {								// ENTER
			inputBuffer[_buffer_index] = '\0';
			_buffer_index = 0;
			pendingCommand = 1;
			if( _echo ) {
				sendChar(10);							// Line Feed
				sendChar(13);							// Carriage Return
			}
		}
		else if( in == 8 ) {							// backspace
			if( _buffer_index > 0 ) _buffer_index--;
			if( _echo ) {
				sendChar(8);
				sendChar(' ');
				sendChar(8);
			}
		}
		else {
			inputBuffer[_buffer_index++] = in;
			if( _echo ) sendChar(in);
		}
		
	}
	else {
		if( in == 8 ) {									// backspace
			_buffer_index--;
			if( _echo ) {
				sendChar(8);
				sendChar(' ');
				sendChar(8);
			}
		} else {
			if( _echo ) sendChar('.');
			_buffer_index++;
			if( in == 13 ) {							// ENTER
				if( _echo ) {
					sendChar(10);						// Line Feed
					sendChar(13);						// Carriage Return
				}
				inputBuffer[INPUT_BUF_SIZE-1] = '\0';
				_buffer_index = 0;
				pendingCommand = LONG_COMMAND;
			}
		}
	}

}

//--------------------------------------------------------------------------------------------------------
// Function:	initUart
//
// Description:	Initialize UART0 for TX and RX
//--------------------------------------------------------------------------------------------------------
void initUart()
{
	unsigned int divisor;		// Used to hold the calculated divisor value
	unsigned int msel;			// Multiplier MSEL[5:0] of PLL_CTL register[14:9]
	unsigned int ssel;			// Divisor SSEL[3:0] of PLL_DIV register[3:0]
	unsigned int csel;			// Divisor CSEL[1:0] of PLL_DIV register[5:4]

	*pUART0_GCTL = UCEN;		// UART Clock enable
	
	// Line Control Setup : 8-bit data, no parity, 1 stop bit
	*pUART0_LCR = 0x0003;


	// Read the MSEL from PLL_CTL register
	msel = (*pPLL_CTL)>>9;		// Read MSEL[5:0] from PLL_CTL
	msel &= 0x3F;				// Clear all bits except msel[5:0]

	// Read SSEL and CSEL from PLL_DIV register
	csel = ssel = *pPLL_DIV;
	ssel &= 0x0F;				// Clear all bits except ssel[3:0]
	csel >>= 4;
	csel &= 0x03;

	/* divisor calculation:
		SCLK = (msel * CLKIN)/ssel if DF = 0
		SCLK = (msel * CLKIN/2)/ssel if DF = 1
		divisor = SCLK/(16 * BAUD_RATE)
	*/
	divisor = ((msel * CLKIN)/(ssel * 16 * BAUD_RATE));
	_ticks_per_char = (coreFrequency * msel)/(csel * BAUD_RATE) * 10;	// length of char: 8bit + 1start + 1stop + 0parity

	if(*pPLL_CTL & 0x1) {		// If DF = 1, CLKIN/2 is going to PLL
		divisor /= 2;			// Divide by 2
		_ticks_per_char /= 2;
	}

	*pUART0_LCR |=	DLAB;		// Enable Divisor Latch Access
	*pUART0_DLL  =	divisor;
	ssync();
	*pUART0_DLH  =	(divisor >> 8);
	ssync();
	*pUART0_LCR &=	~DLAB;		// Disable Divisor Latch Access
	ssync();

	*pUART0_IER =	0x1;		//Enable interrupts for receive

	ssync();
}

#endif
