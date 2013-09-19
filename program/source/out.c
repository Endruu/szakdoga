#include "../headers/out.h"
#include "../headers/logging.h"

#include <stdio.h>
//#include "../headers/uart.h"



void out( const char * string ) {

#ifdef LOGGING_ENABLED
	if( !msgLog( string ) ) {
		printf("! ");
	}
#endif

	printf( string );
}