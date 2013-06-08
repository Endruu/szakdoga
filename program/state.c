#include "global.h"
#include <stdio.h>

filterInfo newFilterInfo() {
	filterInfo r;
	r.iirP			= newIirParameters();
	r.transformP	= newTransformParameters();
	r.iFilter		= NULL;
	r.tFilter		= NULL;
	r.dFilter		= NULL;
	r.filter		= NULL;
	r.wc			= 1;
	r.type			= empty;
	r.subtype		= empty;
	r.fState		= sStart;
	return r;
}

filterInfo copyFilterInfo( filterInfo * fi ) {
	filterInfo r;
	r.iirP			= fi->iirP;
	r.transformP	= fi->transformP;
	r.iFilter		= NULL;
	r.tFilter		= NULL;
	r.dFilter		= NULL;
	r.filter		= NULL;
	r.wc			= 1;
	r.type			= empty;
	r.subtype		= empty;
	r.fState		= fi->fState;
	return r;
}

void deleteFilterInfo( filterInfo * fi ) {
	if(fi->iFilter) deletePzkContainer(fi->iFilter);
	if(fi->tFilter) deletePzkContainer(fi->tFilter);
	if(fi->dFilter) deletePzkContainer(fi->dFilter);
	fi->fState = sStart;
}

int changeState(char code[]) {
	int	tmp
		chars_read = 0;
	
	filterInfo newFilter;
	
	
	
	if( stateVar == sStart && code[0] != 'G' ) {
		//HIBA
	}
	if( stateVar == sIir && code[0] != 'G' ) {
		//HIBA
	}
	
	while( code+chars_read != '\0' ) {
		switch( code[0] ) {
			case 'G':
				if( code[1] == 'I' ) {
					if( tmp = decodeIirInput( code+3 ) ) {
						chars_read += tmp + 3;
						stateVar = sIir;
					} else {
						//HIBA
					}
				}
				break;
			case 'T':
				if( tmp = changeToIir( code+3 ) ) {
		}
	}
	
	
	return chars_read;
}

int decodeIirInput(char code[], filterInfo * fi) {
	char tmp_char;
	float tmp_float;
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
	
	if( !normalizeIirParameters( &ip ) ) {
		error(0);
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