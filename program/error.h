#ifndef _ERROR_H
#define _ERROR_H

#define ERROR_BUFFER_SIZE	10

#define error(e_num)			setError(e_num); return 0
#define errorR(e_num, retval)	setError(e_num); return retval
#define warn(w_num)				setError(-(w_num))

void setError(int error_num);
void clearErrors(void);
int printErrors(char buffer[], int length);

#endif
