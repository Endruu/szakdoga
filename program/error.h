#ifndef _ERROR_H
#define _ERROR_H

#define ERROR_BUFFER_SIZE	10

#define error(e_num)			setError(e_num); return 0
#define error(e_num, retval)	setError(e_num); return retval
#define warn(w_num)				setError(-e_num)

void setError(int error_num);
void clearErrors(void);
void printErrors(char buffer[], int length);

#endif
