#ifndef  _BF_CONSTANTS_H
#define _BF_CONSTANTS_H

//--------------------------------------------------------------------------------------------------------
// Symbolic constants
//--------------------------------------------------------------------------------------------------------
// names for registers in AD1854/AD187 converters
#define INTERNAL_ADC_L0			0
#define INTERNAL_ADC_R0			1
#define INTERNAL_DAC_L0			0
#define INTERNAL_DAC_R0			1

#define AUDIO_RESET_DELAY		0xf00

// SPORT0 word length
#define SLEN_24					0x0017

// DMA flow mode
#define FLOW_1					0x1000

// audio buffersize
#define AUDIO_BUF_SIZE			2				// 2 bytes for 24bit ADC/DAC

#define BAUD_RATE				57600

#define CLKIN					25000000

#define MAX_SEND_TRIES	1000

#define LONG_COMMAND			2

#define ECHO_DEFAULT			0

#define	IM_UART_RX				0x0800
#define	IM_SPORT0_RX			0x0020

#endif
