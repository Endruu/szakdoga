#ifndef  _BF_CONSTANTS_H
#define _BF_CONSTANTS_H

//--------------------------------------------------------------------------------------------------------
// Symbolic constants
//--------------------------------------------------------------------------------------------------------

// ADC/DAC
// names for registers in AD1854/AD187 converters
#define INTERNAL_ADC_L0			0
#define INTERNAL_ADC_R0			1
#define INTERNAL_DAC_L0			0
#define INTERNAL_DAC_R0			1

// SPORT0 word length
#define SLEN_24					0x0017

// DMA flow mode
#define FLOW_1					0x1000

// audio buffersize
#define AUDIO_BUF_SIZE			2				// 2 bytes for 24bit ADC/DAC

#define AUDIO_RESET_DELAY		0xf00

// Clock rate of CPU
#define CLKIN					25000000

// UART
#define	IM_UART_RX				0x0800
#define	IM_SPORT0_RX			0x0020

#define LONG_COMMAND			2		// if input was longer than buffer can handle

#define ECHO_DEFAULT			0		// 0: echo off, 1: echo on
#define ECHO_TOGGLE_CHAR		'?'		// which character toggles echo

#define MAX_SEND_TRIES			10		// how many character send tries before error
#define	MAX_OUTPUT_LENGTH		110;	// max output length for safety, if '\0' is missing from end of string

#define BAUD_RATE				57600

// Other
#undef INPUT_BUF_SIZE
#undef DELAY_SECTION
#undef COEFF_SECTION
#define INPUT_BUF_SIZE			100
#define DELAY_SECTION			section("L1_data_a")
#define COEFF_SECTION			section("L1_data_b")

#undef DEBUG_FUNCTIONS_ENABLED	// disable debug functions
#undef LOGGING_ENABLED			// disable log

#define CPU_DIAGNOSTIC_ENABLED

#endif
