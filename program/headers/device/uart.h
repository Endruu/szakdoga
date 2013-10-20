#ifndef  _UART_H
#define _UART_H

#include "../variables.h"

#ifdef _COMPILE_WITH_BLACKFIN

uint toggleEcho();

void initUart();

uint sendChar(char c);
void receiveString();

#endif

#endif
