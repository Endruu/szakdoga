#include "../headers/communication.h"
#include "../headers/iir_functions.h"
#include "../headers/diagnostics.h"
#include "../headers/logging.h"
#include "../headers/debug.h"

#include <stdlib.h>

void convertToUpper( char * s, int length ) {
	int l;
	for( l = 0; l < length; l++ ) {
		if( s[l] >= 'a' && s[l] <= 'z' ) {
			s[l] = s[l] - 32;
		}
	}

}

int parseInput( char * s, int maxLength ) {
	int length = 0;

	// uppercase conversion and end checking
	while( s[length] != '\0' ) {
		if( length == maxLength ) {
			//hiba
		}
		length++;
	}
	length--;

	convertToUpper(s, 3);
	
	switch( s[0] ) {
	
		case '>' :
			parseInputCommand( s+1, length );
		break;
		
		case '<' :
			parseInputInfo( s+1, length );
		break;
		
		case '#' :
			parseInputDebug( s+1, length );
		break;
		
		default:
		;//hiba
	
	}
	
	return 1;
}

int parseInputInfo( char * s, int l ) {

	if( l > 3 ) {
		//warn
	} else if ( l == 3 && s[2] != CODE_DELIMITER ) {
		//hiba
	}

	convertToUpper(s, l);

	if		( s[0] == 'C' && s[1] == 'W' ) {
		// codeword
		printCodeWord();
	}
	else if	( s[0] == 'C' && s[1] == 'U' ) {
		// cpu usage
		printCpuUsage();
	}
	else if	( s[0] == 'M' && s[1] == 'U' ) {
		// memory usage
		printMemoryUsage();
	}
	
	/* --- not yet implemented
	else if	( s[0] == 'A' && s[1] == 'F' ) {
		printActualFilter();
	}
	else if	( s[0] == 'R' && s[1] == 'I' ) {
		printRoutingInfo();
	}
	*/
	
	else {
		//hiba
	}

	return 1;
}

int parseInputCommand( char * s, int l ) {

	if( l > 2 && s[2] != ':' ) {
		//hiba
	}

	convertToUpper(s, l);
	
	l -= 3;

	if		( s[0] == 'G' && s[1] == 'I' ) {
		cmdGenerateIir( s + 3, l, 0);
	}
	else if	( s[0] == 'M' && s[1] == 'I' ) {
		cmdGenerateIir( s + 3, l, 1);
	}
	
	/* --- not yet implemented
	else if	( s[0] == 'G' && s[1] == 'F' ) {
		cmdGenerateFir( s + 3, l);
	}
	else if	( s[0] == 'S' && s[1] == 'F' ) {
		cmdSelectFilter( s + 3, l);
	}
	else if	( s[0] == 'S' && s[1] == 'O' ) {
		// selects the filter on choosen output
		cmdSelectOutput(  s + 3, l);
	}
	else if	( s[0] == 'R' && s[1] == 'F' ) {
		cmdRouteFilter( s + 3, l);
	}
	*/
	
	else {
		//hiba
	}

	return 1;
}

int parseInputDebug( char * s, int l ) {

#ifdef DEBUG_FUNCTIONS_ENABLED
	if( l > 2 && s[2] != ':' ) {
		//hiba
	}
	
	l -= 3;

	if	( s[0] == 'O' && s[1] == 'L' ) {		// Open Log
		startLog( s + 3 );
	}
	else if	( s[0] == 'C' && s[1] == 'L' ) {	// Close Log
		closeLog();
	}
	else if	( s[0] == 'C' && s[1] == 'M' ) {	// Comment
		msgLog( s + 3 );
		msgLog( "\n" );
	}
	else if	( s[0] == 'E' && s[1] == 'X' ) {	// Exit
		exit(0);
	}
	else if	( s[0] == 'T' && s[1] == 'C' ) {
		runTestcase( s + 3 );
	}
	else if	( s[0] == 'P' && s[1] == 'R' ) {
		simulatePSRespose(  s + 3, l, 0);
	}
	else if	( s[0] == 'S' && s[1] == 'R' ) {
		simulatePSRespose(  s + 3, l, 1);
	}
	else if ( s[0] == 'F' && s[1] == 'I' ) {
		debugFilterInfo( s + 3, l-3 );
	}
	else {
		//hiba
	}

	return 1;

#else
	error(97);
#endif

}
