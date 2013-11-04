#include "../headers/device/device.h"		//prototypes

#ifdef _COMPILE_WITH_BLACKFIN

#include "../../headers/device/uart.h"
#include <blackfin.h>

void initializeDevice() {

/*
	0. Configure coreFrequency
	1. Configure PORT F
	2. Configure SPORT0
	3. Reset audio (ADC and DAC)
	4. Configure DMA for ADC and DAC
	5. Configure interrupts
	6. Initialize UART
	7. Enable DMA and interrupts

*/
//--------------------------------------------------------------------------------------------------------
// 0. Configure coreFrequency
//--------------------------------------------------------------------------------------------------------

	unsigned int msel;			// Multiplier MSEL[5:0] of PLL_CTL register[14:9]
	unsigned int csel;			// Divisor CSEL[1:0] of PLL_DIV register[5:4]
	
	// Read the MSEL from PLL_CTL register
	msel = (*pPLL_CTL) >> 9;	// Read MSEL[5:0] from PLL_CTL
	msel &= 0x3F;				// Clear all bits except msel[5:0]

	// Read CSEL from PLL_DIV register
	csel = *pPLL_DIV >> 4;
	csel &= 0x03;				// Clear all bits except csel[1:0]
	
	coreFrequency = ( msel * CLKIN ) / csel;
	
	if(*pPLL_CTL & 0x1) {		// If DF = 1, CLKIN/2 is going to PLL
		coreFrequency /= 2;
	}

//--------------------------------------------------------------------------------------------------------
// 1. Configure PORTF
//--------------------------------------------------------------------------------------------------------
	// PORTF[15:13]	: CAN interface (Disabled)
	// PORTF[12]	: Audio Reset (Active Low)
	// PORTF[11:6]	: LED[6-1]
	// PORTF[5:2]	: Button[4-1]
	// PORTF[1]		: UART0 RX
	// PORTF[0]		: UART0 TX
	
	int temp;
	// configure programmable flags
	// set PORTF Function Enable Register (need workaround)
	temp = *pPORTF_FER;
	temp++;
    *pPORTF_FER = 0x0000;
    *pPORTF_FER = 0x0003;		// enable uart0 TX and RX function

    // set PORTF direction register
    *pPORTFIO_DIR = 0x1FC0;		// reg[15:0] : IIIO OOOO OOII IISS	(Input, Output, Special)
        
   	// set PORTF input enable register
    *pPORTFIO_INEN = 0x003C;	// reg[15:0] : DDDE OOOO OOEE EESS	(Disabled, Enabled, Output)
         
	// set PORTF clear register
    *pPORTFIO_CLEAR = 0x0FC0;	// clear output pins

//--------------------------------------------------------------------------------------------------------
// 2. Configure Sport0 for I2S mode, to transmit/receive data to/from the ADC/DAC.
//--------------------------------------------------------------------------------------------------------
	// use Port J (with its default mux settings)
	
	// xFSR		: Frame Sync Required (for every data word)
	// LxFS		: Low x Frame Sync 
	// xCKFE	: Clock drive/sample edge (data driven on the falling edge, externally generated frame syncs sampled on rising edge)
	// SLEN_24	: 24-bit data format
	// xSFSE	: Stereo Frame Sync Enable
	
	// Sport0 receive configuration:
	// External CLK (~IRCLK), External Frame sync (~ITFS), MSB first (~RLSBIT), Active Low
	// 24-bit data, Stereo frame sync enable
	*pSPORT0_RCR1 = RFSR | LRFS | RCKFE;
	*pSPORT0_RCR2 = SLEN_24 | RSFSE;
	
	// Sport0 transmit configuration:
	// External CLK, External Frame sync, MSB first, Active Low
	// 24-bit data, Stereo frame sync enable
	*pSPORT0_TCR1 = TFSR | LTFS | TCKFE;
	*pSPORT0_TCR2 = SLEN_24 | TSFSE;	

//--------------------------------------------------------------------------------------------------------
// 3. Reset audio (ADC and DAC)
//--------------------------------------------------------------------------------------------------------
	// clear port f set register
    *pPORTFIO_CLEAR = PF12;		// 0: reset
	
	// give some time for reset to take affect
    for(temp = 0; temp< AUDIO_RESET_DELAY; temp++) {};
 	
    // set port f set register
    *pPORTFIO_SET = PF12;		// 1: reset
	
//--------------------------------------------------------------------------------------------------------
// 4. Configure DMA3 and DMA4
//--------------------------------------------------------------------------------------------------------
	// Configure DMA3: receive, autobuffer mode, 32-bit, interrupt on completion
	*pDMA3_CONFIG = WNR | WDSIZE_32 | DI_EN | FLOW_1;
	// Start address of data buffer
	*pDMA3_START_ADDR = aRxBuffer;
	// DMA loop count
	*pDMA3_X_COUNT = 2;
	// DMA loop address increment
	*pDMA3_X_MODIFY = 4;
	
	// Configure DMA4: transmit, autobuffer mode, 32-bit
	*pDMA4_CONFIG = WDSIZE_32 | FLOW_1;
	// Start address of data buffer
	*pDMA4_START_ADDR = aTxBuffer;
	// DMA loop count
	*pDMA4_X_COUNT = 2;
	// DMA loop address increment
	*pDMA4_X_MODIFY = 4;
	
//--------------------------------------------------------------------------------------------------------
// 5. Configure Interrupts
//--------------------------------------------------------------------------------------------------------
	// Set interrupt priorities:
	*pSIC_IAR0 = 0xFF2FFFFF;	// ADC RX (SPORT0, DMA3)	2 => IVG9
	*pSIC_IAR1 = 0xFFFF3FFF;	// UART0 RX					3 => IVG10
	*pSIC_IAR2 = 0xFFFFFFFF;	// all other:
	*pSIC_IAR3 = 0xFFFFFFFF;	// IVG15

	// assign ISRs to interrupt vectors:
	
	// Sport0 RX ISR -> IVG 9
	register_handler(ik_ivg9, Sport0_RX_ISR);
	// UART0 RX ISR -> IVG 10
	register_handler(ik_ivg10, Uart0_RX_ISR);

//--------------------------------------------------------------------------------------------------------
// 6. Initialize UART0
//--------------------------------------------------------------------------------------------------------
	initUart();		// from uart.h

//--------------------------------------------------------------------------------------------------------
// 7. Enable DMA3, DMA4, Sport0_TX, Sport0_RX, and interrupts
//--------------------------------------------------------------------------------------------------------
	// enable DMAs
	*pDMA4_CONFIG	|= DMAEN;
	*pDMA3_CONFIG	|= DMAEN;
	
	// enable Sport0 TX and RX
	*pSPORT0_TCR1 	|= TSPEN;
	*pSPORT0_RCR1 	|= RSPEN;

	// enable UART0 RX and Sport0 RX interrupts
	*pSIC_IMASK = IM_UART0_RX | IM_SPORT0_RX;
	
}

#endif
