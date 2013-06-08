#include "global.h"		//prototypes

#ifdef _COMPILE_WITH_BLACKFIN
#include <blackfin.h>	//symbolic register names
#endif


//--------------------------------------------------------------------------------------------------------
// Definition of global variables
//--------------------------------------------------------------------------------------------------------

int aLeftIn;
int aRightIn;
int aLeftOut		= 0;
int aRightOut		= 0;

int aRxBuffer[AUDIO_BUF_SIZE];
int aTxBuffer[AUDIO_BUF_SIZE];
int channelDelay	= 0;

pzkContainer pzkList[3];

char uart_buffer[UART_BUF_SIZE];

uint changeFilterRequest = 1;

filterInfo filterR;
#ifdef _COMPILE_WITH_BLACKFIN
section("L1_data_a") DELAY_TYPE delayLineR[DELAY_SIZE];
section("L1_data_b") COEFF_TYPE coeffLine1[COEFF_SIZE];
section("L1_data_b") COEFF_TYPE coeffLine2[COEFF_SIZE];
#else
DELAY_TYPE delayLineR[DELAY_SIZE];
COEFF_TYPE coeffLine1[COEFF_SIZE];
COEFF_TYPE coeffLine2[COEFF_SIZE];
#endif

COEFF_TYPE * coeffLineR = coeffLine1;
COEFF_TYPE * coeffLineTemp = coeffLine2;

unsigned int interruptBackup;

#ifdef _COMPILE_WITH_BLACKFIN

//--------------------------------------------------------------------------------------------------------
// Function:	Init_Flags
//
// Description:	Configure PORTF flags to control ADC and DAC RESETs
//--------------------------------------------------------------------------------------------------------
void Init_Flags()
{
	int temp;
	// configure programmable flags
	// set PORTF function enable register (need workaround)
	temp = *pPORTF_FER;
	temp++;
    *pPORTF_FER = 0x0000;
    *pPORTF_FER = 0x0000;

    // set PORTF direction register
    *pPORTFIO_DIR = 0x1FC0;
        
   	// set PORTF input enable register
    *pPORTFIO_INEN = 0x003C;
         
	// set PORTF clear register
    *pPORTFIO_CLEAR = 0x0FC0;
}


//--------------------------------------------------------------------------------------------------------
// Function:	Audio_Reset
//
// Description:	This function Resets the ADC and DAC.
//--------------------------------------------------------------------------------------------------------
void Audio_Reset()
{
	int i;
	// give some time for reset to take affect
    for(i = 0; i< AUDIO_RESET_DELAY; i++) {};
 	
    // set port f set register
    *pPORTFIO_SET = PF12;

}

//--------------------------------------------------------------------------------------------------------
// Function:	Init_Sport0
//
// Description:	Configure Sport0 for I2S mode, to transmit/receive data
//				to/from the ADC/DAC.Configure Sport for external clocks and
//				frame syncs.
//--------------------------------------------------------------------------------------------------------
void Init_Sport0()
{
	// Sport0 receive configuration
	// External CLK, External Frame sync, MSB first, Active Low
	// 24-bit data, Secondary side enable, Stereo frame sync enable
	*pSPORT0_RCR1 = RFSR | LRFS | RCKFE;
	*pSPORT0_RCR2 = SLEN_24 | RSFSE;
	
	// Sport0 transmit configuration
	// External CLK, External Frame sync, MSB first, Active Low
	// 24-bit data, Secondary side enable, Stereo frame sync enable
	*pSPORT0_TCR1 = TFSR | LTFS | TCKFE;
	*pSPORT0_TCR2 = SLEN_24 | TSFSE;	
}

//--------------------------------------------------------------------------------------------------------
// Function:	Init_DMA
//
// Description:	Initialize DMA3 in autobuffer mode to receive and DMA4 in
//				autobuffer mode to transmit
//--------------------------------------------------------------------------------------------------------
void Init_DMA()
{
	// Configure DMA3
	// 32-bit transfers, Interrupt on completion, Autobuffer mode
	*pDMA3_CONFIG = WNR | WDSIZE_32 | DI_EN | FLOW_1;
	// Start address of data buffer
	*pDMA3_START_ADDR = aRxBuffer;
	// DMA loop count
	*pDMA3_X_COUNT = 2;
	// DMA loop address increment
	*pDMA3_X_MODIFY = 4;
	

	// Configure DMA4
	// 32-bit transfers, Autobuffer mode
	*pDMA4_CONFIG = WDSIZE_32 | FLOW_1;
	// Start address of data buffer
	*pDMA4_START_ADDR = aTxBuffer;
	// DMA loop count
	*pDMA4_X_COUNT = 2;
	// DMA loop address increment
	*pDMA4_X_MODIFY = 4;

}

//--------------------------------------------------------------------------------------------------------
// Function:	Enable_DMA_Sport
//
// Description:	Enable DMA3, DMA4, Sport0 TX and Sport0 RX
//--------------------------------------------------------------------------------------------------------
void Enable_DMA_Sport0()
{
	// enable DMAs
	*pDMA4_CONFIG	= (*pDMA4_CONFIG | DMAEN);
	*pDMA3_CONFIG	= (*pDMA3_CONFIG | DMAEN);
	
	// enable Sport0 TX and RX
	*pSPORT0_TCR1 	= (*pSPORT0_TCR1 | TSPEN);
	*pSPORT0_RCR1 	= (*pSPORT0_RCR1 | RSPEN);
}

//--------------------------------------------------------------------------------------------------------
// Function:	Init_Interrupts
//
// Description:	Initialize Interrupt for Sport0 RX
//--------------------------------------------------------------------------------------------------------
void Init_Interrupts()
{
	// Set Sport0 RX (DMA3) interrupt priority to 2 = IVG9 
	*pSIC_IAR0 = 0xff2fffff;
	*pSIC_IAR1 = 0xffffffff;
	*pSIC_IAR2 = 0xffffffff;
	*pSIC_IAR3 = 0xffffffff;

	// assign ISRs to interrupt vectors
	// Sport0 RX ISR -> IVG 9
	register_handler(ik_ivg9, Sport0_RX_ISR);

	// enable Sport0 RX interrupt
	*pSIC_IMASK = 0x00000020;
}


//--------------------------------------------------------------------------------------------------------
// Function:	Init_All
//
// Description:	Aggregates all of the initializating functions
//--------------------------------------------------------------------------------------------------------
void Init_Device()
{
	Init_Flags();
	Audio_Reset();
	Init_Sport0();
	Init_DMA();
	//Init_Interrupts();
	Enable_DMA_Sport0();
}

#endif
