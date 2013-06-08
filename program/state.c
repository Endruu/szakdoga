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
	r.type		= empty;
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
	r.type		= empty;
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
	
	filterInfo newFilter = copyFilterInfo( &filterR );
	
	while( code+chars_read != '\0' ) {
		switch( code[chars_read] ) {
			case 'G':
				if( code[chars_read+1] == 'I' ) {
					if( tmp = decodeIirInput( code+chars_read+3, &newFilter ) ) {
						chars_read += tmp + 3;
						newFilter.sState = sReferent;
					} else {
						error(0);
					}
				} else if( code[chars_read+1] == 'F' ) {
					// FIR
				} else {
					error(0);
				}
				break;
			case 'T':
				if( tmp = decodeTransformInput( code+chars_read+2, &newFilter ) ) {
					chars_read += tmp + 2;
					if( newFilter.sState > sTransform ) {
						newFilter.sState = sTransform;
					}
				} else {
					error(0);
				}
				break;
			default:
				error(0);
		}
	}
	
	if( newFilter.supertype == iir ) {
		switch( newFilter.sState ) {
			case sReferent:
				if( createReferentFilter( &newFilter );
			case sTransform:
				if( newFilter.iFilter == NULL ) {	
					error(0);
				}
				if( !transformFilter( &newFilter ) ) {
					deleteFilterInfo( &newFilter );
					error(0);
				}
			case sDigitalize:
				if( newFilter.tFilter == NULL ) {
					deleteFilterInfo( &newFilter );
					error(0); 
				}
				if( !digitalizeFilter( &newFilter ) ) {
					deleteFilterInfo( &newFilter );
					error(0);
				}
			case sImplement:
				if( newFilter.dFilter == NULL ) {
					deleteFilterInfo( &newFilter );
					error(0); 
				}
				if( !implementFilter( &newFilter ) ) {
					deleteFilterInfo( &newFilter );
					error(0); 
				}
			break;
		}
		
		switch( newFilter.sState ) {
			case sTransform:
				newFilter.iFilter = filterR.iFilter;
				filterR.iFilter = NULL;
			case sDigitalize:
				newFilter.tFilter = filterR.tFilter;
				filterR.tFilter = NULL;
			case sImplement:
				newFilter.dFilter = filterR.dFilter;
				filterR.dFilter = NULL;
			break;
		}
		deleteFilterInfo( &filterR );
		filterR = newFilterInfo;
		return chars_read;
		
	} else if( newFilter.supertype == fir ) {
	} else {
		error(0);
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

	fi->iirP = ip;
	fi->subtype = filter;
	
	return chars_read;
}