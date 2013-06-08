#include "global.h"

#ifdef _COMPILE_WITH_BLACKFIN

#define NO_ETSI_BUILTINS
#include <libetsi.h>
#include <stdio.h>

fract32 acc = MAX_32;

void resetFilter()
{
	if( delayLine != NULL ) {
		free(delayLine);
		delayLine = NULL;
	}
	
	if( coefLine != NULL ) {
		free(coefLine);
		coefLine = NULL;
	}
	
	filterFunction = &fPassThrough;
	stages = 0;
}

void fPassThrough() {
	aRightOut	= aRightIn;
	aLeftOut	= aRightIn;
}

void fDirect1() {
	int i;
	delayLineR[0] = MAX_16;
	delayLineR[1] = MAX_16;
	
	Overflow = 1; Carry = 1;
	L_add_c(delayLineR[0], delayLineR[1]);
	printf("ACC: %d - OF: %d - CY: %d\n---\n", acc, Overflow, Carry);
	
	
	
	//printf("ACC: %d - OF: %d - CY: %d\n", acc, Overflow, Carry);
	for(i=0; i<3; i++) {
		//acc = L_mac(acc, delayLineR[0], delayLineR[1]);
		acc = L_macNs(acc, delayLineR[0], delayLineR[1]);
		printf("ACC: %d - OF: %d - CY: %d\n", acc, Overflow, Carry);
		Overflow = 1; Carry = 1;
		printf("ACC: %d - OF: %d - CY: %d\n", acc, Overflow, Carry);
		acc = L_sat(acc);
		printf("ACC: %d - OF: %d - CY: %d\n", acc, Overflow, Carry);
		
	}
	
	printf("ACC: %d - OF: %d - CY: %d\n", acc, Overflow, Carry);
	
	/*
	acc = 0xAFFFFFFF;	//+MAX
	b = 1;
	
	Overflow = 0;
	
	acc = 0x80000000;	//-MAX
	b = 0xFFFF;
	acc = L_macNs(acc, a, b);
	printf("ACC: %d - OF: %d - CY: %d\n", acc, Overflow, Carry);
	Overflow = 0;
	
	acc = 0;
	b = 0xFFFF;
	acc = L_macNs(acc, a, b);
	printf("ACC: %d - OF: %d - CY: %d\n", acc, Overflow, Carry);*/
}

#endif
