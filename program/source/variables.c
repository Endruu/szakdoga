#include "../headers/variables.h"

//--------------------------------------------------------------------------------------------------------
// Definition of global variables
//--------------------------------------------------------------------------------------------------------

char inputBuffer[INPUT_BUF_SIZE];
char pendingCommand	= 0;

unsigned int interruptBackup;

DELAY_SECTION DELAY_TYPE delayLine[DELAY_SIZE];
COEFF_SECTION COEFF_TYPE coeffLines[2][COEFF_SIZE];

filterInfo filterBank[2];

int actualFilter = 0;
int tmpFilter = 1;

int coreFrequency = 1;
