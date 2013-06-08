#include "error.h"
#include <stdio.h>

int _error_container[ERROR_BUFFER_SIZE];
int _errors_occurred = 0;

void setError(int error_num) {
	if( _errors_occurred < ERROR_BUFFER_SIZE ) {
		_error_container[_errors_occurred] = error_num;
	} else {
		_error_container[ERROR_BUFFER_SIZE-1] = error_num;
	}
	_errors_occurred++;
}

void clearErrors() {
	_errors_occurred = 0;
}

int printErrors(char buffer[], int length) {
	int i, ptr = 0;
	
	if( length < (ERROR_BUFFER_SIZE * 6 + 11) || buffer == NULL ) {	//buffer too small or undefined
		return 0;
	} else {
	
		for(i=0; i < _errors_occurred && i < ERROR_BUFFER_SIZE; i++) {
			if( _error_container[i] < 0 ) {
				if( sprintf( buffer+ptr, "W%04X\n", (unsigned int)(-_error_container[i]) ) < 0 ) {
					return 0;
				}
			} else {
				if( sprintf( buffer+ptr, "E%04X\n", (unsigned int)(_error_container[i]) ) < 0 ) {
					return 0;
				}
			}
			ptr += 6;
		}
		
		if( _errors_occurred > ERROR_BUFFER_SIZE ) {
			i = _errors_occurred - ERROR_BUFFER_SIZE;
			if( i > 999 ) i = 999;
		} else {
			i = 0;
		}
		if( sprintf( buffer+ptr, "lost: %3u\n\0", (unsigned int)i ) < 0 ) {
			return 0;
		}
	}
	return ptr + 11;
}