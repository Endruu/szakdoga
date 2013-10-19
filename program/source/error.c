#include "../headers/error.h"
#include "../headers/constants.h"

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

void printErrors() {
	int i;
	char buffer[20];
	
	for( i=0; i < _errors_occurred && i < ERROR_BUFFER_SIZE; i++ ) {
		if( _error_container[i] < 0 ) {
			if( sprintf( buffer, "W-%04d\n", (unsigned int)(-_error_container[i]) ) ) {
				out( buffer );
			}
		} else {
			if( sprintf( buffer, "E-%04d\n", (unsigned int)(_error_container[i]) )  ) {
				out( buffer );
			}
		}
	}
	
	if( _errors_occurred > ERROR_BUFFER_SIZE ) {
		i = _errors_occurred - ERROR_BUFFER_SIZE;
		if( i > 999 ) i = 999;

		if( sprintf( buffer, "lost: %3u\n", (unsigned int)i ) ) {
			out( buffer );
		}
	}

}

int getErrors() {
	return _errors_occurred;
}
