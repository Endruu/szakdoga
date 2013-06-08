#ifndef  _GLOBAL_FUNC_H 
#define _GLOBAL_FUNC_H 


//--------------------------------------------------------------------------------------------------------
// Header files																			
//--------------------------------------------------------------------------------------------------------
#include "global_var.h"		//global variables

#ifdef _COMPILE_WITH_BLACKFIN
#include <sys\exception.h>	//interrupt handling
#else

#define EX_INTERRUPT_HANDLER(_SG)

#endif


//--------------------------------------------------------------------------------------------------------
// Prototypes
//--------------------------------------------------------------------------------------------------------
// in file initialize.c
void Init_Flags(void);
void Audio_Reset(void);
void Init_Sport0(void);
void Init_DMA(void);
void Init_Interrupts(void);
void Enable_DMA_Sport0(void);
void Init_All(void);						// the previous functions aggregated

// in file interrupts.c
EX_INTERRUPT_HANDLER(Sport0_RX_ISR);

// in file ecomplex.c
real cabs2(complex c);
complex cmul2(real a, complex b);
complex cadd2(real a, complex b);
complex csub2(real a, complex b);
complex csqrt(complex c);

uint cisnull(complex  c);
uint cisreal(complex  c);
uint cisimag(complex  c);
uint cisequal(complex  a, complex b);

// in file filters.c
void resetFilter(void);
void fPassThrough(void);
void fDirect1(void);

// in file implement.c
void iDirect1(pzkContainer * pzk);

// in file pzk_container.c
pzkContainer *	createPzkContainer(uint np, uint nz);
uint	shrinkPzkContainer(pzkContainer * pzk, uint min);
void	deletePzkContainer(pzkContainer * pzk);
void	printPzkContainer(pzkContainer * pzk);
void print4Matlab(pzkContainer * pzk);

uint addPole(pzkContainer * pzk, complex pole);
uint addZero(pzkContainer * pzk, complex zero);

uint countPoles(pzkContainer * pzk);
uint countZeros(pzkContainer * pzk);
uint countBiquads(pzkContainer * pzk);

// in file generate.c
pzkContainer * createButterworth(uint n, real e0);
pzkContainer * createChebyshev1(uint n, real e0);
pzkContainer * createChebyshev2(uint n, real Os, real d2);

// in file transform.c
pzkContainer * t2lp(pzkContainer * pzk, real w0);
pzkContainer * t2hp(pzkContainer * pzk, real w0);
pzkContainer * t2bp(pzkContainer * pzk, real w0, real dw);
pzkContainer * t2bs(pzkContainer * pzk, real w0, real dw);

pzkContainer * bilinear(pzkContainer * pzk, real pwf);
real getPrewarpFreq(real radps, real samplingTime);

#endif
