#include "global.h"
#include <stdio.h>

int changeState(char code[]) {
	int	tmp
		chars_read = 0;
	
	if( stateVar == sStart && code[0] != 'G' ) {
		//HIBA
	}
	if( stateVar == sIir && code[0] != 'G' ) {
		//HIBA
	}
	
	switch( code[0] ) {
		case 'G':
			if( code[1] == 'I' ) {
				if( chars_read = changeToIir( code+3 ) ) {
					chars_read += 3;
					stateVar = sIir;
				} else {
					//HIBA
				}
			}
			break;
	}
	return chars_read;
}

int changeToIir(char code[]) {
	char tmp_char;
	float tmp_float;
	double tmp_double;
	int i, chars_scanned = 0, chars_read;
	filterTypes filter;
	iirParameters ip = newIirParameters();
	pzkContainer * tmp_pzk = NULL;
	
	if(code[0] == 'B' && code[1] == 'W') {
		filter = butterworth;
	} else if(code[0] == 'C' && code[1] == '1') {
		filter = chebyshev1;
	} else if(code[0] == 'C' && code[1] == '2') {
		filter = chebyshev2;
	} else {
		//HIBA
	}
	
	chars_read = 2;
	
	for(i=0; i < 6; i++) {
		if( sscanf(code + chars_read, "%c%g%n", &tmp_char, &tmp_float, &chars_scanned) == 3 ) {
			switch(tmp_char) {
				case 'n' :
				case 'N' :
					ip.n = (uint)tmp_float; break;
				case 'a' :
				case 'A' :
					ip.ac = tmp_float; break;
				case 'b' :
				case 'B' :
					ip.as = tmp_float; break;
				case 'w' :
				case 'W' :
					ip.ws = tmp_float; break;
				case 'l' :
				case 'L' :
					ip.inDb = 0; break;
				default:
					//HIBA
			}
			chars_read += chars_scanned;
		} else if( tmp_char == CODE_DELIMITER || tmp_char == "\0" ) {
			chars_read += chars_scanned;
			if( tmp_char == "\0" ) chars_read--;
			i = -1;		// signal that the delimiter or the end has been found
			break;
		} else {
			//HIBA
		}
	}
	if( i != -1 ) {		// if the delimiter or the end has not been found
		//HIBA
	}
	
	tmp_double = normalizeIirParameters( &ip );
	if( tmp_double == 0 ) {
		//HIBA
	} else if( tmp_double < 0 ) {
		//WARN
		tmp_double *= -1;
	}
	
	
	switch( filter ) {
		case butterworth:
			if( convertParametersForButterworth( &ip ) ) {
				tmp_pzk = createButterworth(ip.n, ip.e0) );
			} else {
				//HIBA
			}
			break;
			
		case chebyshev1:
			if( convertParametersForChebyshev1( &ip ) ) {
				tmp_pzk = createChebyshev1(ip.n, ip.e0) );
			} else {
				//HIBA
			}
			break;
		case chebyshev2:
			if( convertParametersForChebyshev2( &ip ) ) {
				tmp_pzk = createChebyshev2(ip.n, ip.ws, ip.as) );
			} else {
				//HIBA
			}
			break;
	}
	
	if( tmp_pzk != NULL ) {
		pzkList[0] = tmp_pzk;
		fInfo.iirParameters = ip;
		fInfo.subtype = filter;
		fInfo.wc = tmp_double;
	} else {
		//HIBA
	}
	
	return chars_read;
}