#ifndef  _FILTERINFO_H 
#define _FILTERINFO_H 

#include "types.h"

filterInfo defaultIirFilterInfo();
filterInfo copyFilterInfo( filterInfo * fi );

transformParameters defaultTransformParameters();
iirParameters defaultIirParameters();

#endif
