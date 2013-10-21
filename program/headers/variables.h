#ifndef  _VARIABLES_H
#define _VARIABLES_H

#include "types.h"
#include "error.h"

extern char inputBuffer[];

// pending string from uart
extern char pendingCommand;

extern unsigned int interruptBackup;

// delay line, and coeff array
extern DELAY_TYPE delayLine[];
extern COEFF_TYPE coeffLines[][COEFF_SIZE];

extern filterInfo filterBank[];

extern int actualFilter;
extern int tmpFilter;

extern int coreFrequency;

#endif
