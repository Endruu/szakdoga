#include "../headers/out.h"
#include "../headers/logging.h"

#include <stdio.h>
#include "../headers/device/uart.h"



void out( const char * string ) {
	int i;
#ifdef LOGGING_ENABLED
	if( !msgLog( string ) ) {
		printf("! ");
	}
#endif

#ifdef COMPILE_WITH_BLACKFIN
	for( i = 0; i < MAX_OUTPUT_LENGTH; i++ ) {
		if( string[i] != '\0' ) {
			sendChar( string[i] );
		} else {
			break;
		}
	}
#else
	printf( string );
#endif
}