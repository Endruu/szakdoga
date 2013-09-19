#include "constants.h"

#ifndef _LOGGING_H
#define _LOGGING_H

#define LOGGING_MODE	"w"		// "a" - append, "w" - write

int msgLog( const char * msg );
void startLog( const char * filename );
void closeLog();

#endif	// guard
