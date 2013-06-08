#include "global.h"

#ifdef _COMPILE_WITH_BLACKFIN

#include <blackfin.h>

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
	delayLineR[0] = aRightIn >> 8;
	aRightOut = (*filterR.filter)(coeffLineR, delayLineR, filterR.stages) >> 8;

}

#endif
