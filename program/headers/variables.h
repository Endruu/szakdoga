#ifndef  _VARIABLES_H
#define _VARIABLES_H

#include "types.h"
#include "error.h"

extern int aLeftIn;
extern int aRightIn;
extern int aLeftOut;
extern int aRightOut;

// DMA buffers for audio
extern int aRxBuffer[];
extern int aTxBuffer[];
extern int channelDelay;

extern char inputBuffer[];

// pending string from uart
extern uint uartRequest;

extern unsigned int interruptBackup;


// delay line, and coeff array
extern DELAY_TYPE delayLine[];
extern COEFF_TYPE coeffLines[][COEFF_SIZE];

extern filterInfo filterBank[];

extern int actualFilter;
extern int tmpFilter;

#endif
