#include "../headers/variables.h"

//--------------------------------------------------------------------------------------------------------
// Definition of global variables
//--------------------------------------------------------------------------------------------------------

char inputBuffer[INPUT_BUF_SIZE];

uint uartRequest	= 0;
unsigned int interruptBackup;

#ifdef _COMPILE_WITH_BLACKFIN
section("L1_data_a") DELAY_TYPE delayLine[DELAY_SIZE];
section("L1_data_b") COEFF_TYPE coeffLines[2][COEFF_SIZE];
#else
DELAY_TYPE delayLine[DELAY_SIZE];
COEFF_TYPE coeffLines[2][COEFF_SIZE];
#endif

filterInfo filterBank[2];

int actualFilter = 0;
int tmpFilter = 1;
