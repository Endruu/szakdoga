#include "../headers/global.h"

#include <stdio.h>

void out( char * string ) {
	printf( string );
}

void parseInputInfo( char * s, int l ) {

	if( l > 3 ) {
		//warn
	} else if ( l == 3 && s[2] != CODE_DELIMITER ) {
		//hiba
	}

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
	
	// --- not yet implemented
	else if	( s[0] == 'A' && s[1] == 'F' ) {
		printActualFilter();
	}
	else if	( s[0] == 'R' && s[1] == 'I' ) {
		printRoutingInfo();
	}
	
	else {
		//hiba
	}

}

void parseInputDebug( char * s, int l ) {
	
	if( l > 3 ) {
		//warn
	} else if ( l == 3 && s[2] != CODE_DELIMITER ) {
		//hiba
	}
	
	if ( s[0] == 'F' && s[1] == 'I' ) {
		debugFilterInfo();
	}
	else {
		//hiba
	}
	
}

void parseInputCommand( char * s, int l ) {

	if( l > 2 && s[2] != ':' ) {
		//hiba
	}
	
	l -= 3;

	if		( s[0] == 'G' && s[1] == 'F' ) {
		cmdGenerateFilter( s + 3, l);
	}
	
	// --- not yet implemented
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
	
	else {
		//hiba
	}

}

void parseInputTest( char * s, int l ) {

	if( l > 2 && s[2] != ':' ) {
		//hiba
	}
	
	l -= 3;
	
	if		( s[0] == 'T' && s[1] == 'C' ) {
		runTestcase( s + 3, l);
	}
	else if	( s[0] == 'I' && s[1] == 'R' ) {
		runImpulse(  s + 3, l);
	}
	else if	( s[0] == 'R' && s[1] == 'S' ) {
		runSimulation( s + 3, l);
	}
	else {
		//hiba
	}
}

void parseInput( char * s, int maxLength ) {
	int length = 0;

	// uppercase conversion and end checking
	while( s[length] != '\0' ) {
		if( length == maxLength ) {
			//hiba
		}
		if( s[length] >= 'a' && s[length] <= 'z' ) {
			s[length] = s[length] - 32;
		}
		length++;
	}

	length--;
	
	switch( s[0] ) {
	
		case '>' :
			parseInputCommand( s+1, length );
		break;
		
		case '<' :
			parseInputInfo( s+1, length );
		break;
		
		case '?' :
			parseInputDebug( s+1, length );
		break;
		
		case '#' :
			parseInputTest( s+1, length );
		break;
		
		default:
		//hiba
	
	}
	
}