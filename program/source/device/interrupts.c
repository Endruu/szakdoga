#include "../../headers/device/interrupts.h"

#ifdef _COMPILE_WITH_BLACKFIN

#include "../../headers/device/uart.h"

int aLeftIn;
int aRightIn;
int aLeftOut		= 0;
int aRightOut		= 0;

// DMA buffers for audio
int aRxBuffer[AUDIO_BUF_SIZE];
int aTxBuffer[AUDIO_BUF_SIZE];
int channelDelay	= 0;

//--------------------------------------------------------------------------------------------------------
// Function:	Sport0_RX_ISR
//
// Description: This ISR is executed after a complete frame of input data
//				has been received. The new samples are stored in
//				iChannel0LeftIn and iChannel0RightIn.  Then the function
//				Process_Data() is called in which user code can be executed.
//				After that the processed values are copied from the
//				variables iChannel0LeftOut and iChannel0RightOut into the
//				DMA transmit buffer.
//--------------------------------------------------------------------------------------------------------
EX_INTERRUPT_HANDLER(Sport0_RX_ISR)
{
	// confirm interrupt handling
	*pDMA3_IRQ_STATUS = 0x0001;

	// copy previous processed data from variables into dma output buffer
	aTxBuffer[INTERNAL_DAC_L0] = channelDelay;
	aTxBuffer[INTERNAL_DAC_R0] = aRightOut;
	channelDelay = aLeftOut;
	
	// copy input data from dma input buffer into variables
	aLeftIn		= aRxBuffer[INTERNAL_ADC_L0];
	aRightIn	= aRxBuffer[INTERNAL_ADC_R0];
	
	// run filter
	aLeftOut = aRightIn;
	//aRightOut = aLeftIn;
	aRightOut = (filterBank[actualFilter].filter)(aRightIn >> 8, coeffLineR, delayLineR) >> 8;

}

EX_INTERRUPT_HANDLER(Uart0_RX_ISR)
{
	receiveString();
}

#endif
