#include "../headers/global.h"

#include <stdio.h>

void out( char * string ) {
	printf( string );
}

void parseInput( char * s ) {
	int tmp = 0;

	// to upper
	while( s[tmp] != '\0' ) {	// +tmp limit!
		if( s[tmp] >= 'a' && s[tmp] <= 'z' ) {
			s[tmp] = s[tmp] - 32;
		}
		tmp++;
	}

	if( s[0] == '>' ) {			// change parameters
	
		if( s[1] == 'G' ) {		// generate filter
			if( s[3] == 'I' ) {
				// iir
			}
			else if( s[3] == 'F' ) {
				// fir
			}
			else {
				//hiba
			}
		}
		else if( s[1] == 'C' ) {
			// change channel
		}
		else {
			//hiba
		}
		
	}
	
	else if( s[0] == '<' ) {	// request info
	
		if		( s[1] == 'C' && s[2] == 'W' ) {
			// codeword
			printCodeWord();
		}
		else if	( s[1] == 'C' && s[2] == 'U' ) {
			// cpu usage
			printTick( filterR );
		}
		else if	( s[1] == 'M' && s[2] == 'U' ) {
			// memory usage
			printMem( filterR );
		}
		else if	( s[1] == 'A' && s[2] == 'C' ) {
			// actual channel
			printActualChannel();
		}
		else {
			//hiba
		}
		
	}
	
	/* else if( s[0] == '?' ) { // testcase } */
	else {
		//hiba
	}
}