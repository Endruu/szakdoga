#ifndef _INTERRUPTS_H
#define _INTERRUPTS_H

#include "../variables.h"

#ifdef _COMPILE_WITH_BLACKFIN

#include <blackfin.h>

EX_INTERRUPT_HANDLER(Sport0_RX_ISR);	// ADC Interrupt
EX_INTERRUPT_HANDLER(Uart0_RX_ISR);		// UART Receive interrupt

#endif

#endif
