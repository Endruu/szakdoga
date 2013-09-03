#ifndef  _GLOBAL_FUNC_H 
#define _GLOBAL_FUNC_H 


//--------------------------------------------------------------------------------------------------------
// Header files	and macros																	
//--------------------------------------------------------------------------------------------------------
#include "global_var.h"		// global variables
#include "error.h"			// error handling

#ifdef _COMPILE_WITH_BLACKFIN
#include <sys\exception.h>	//interrupt handling
#define CLI()	interruptBackup = cli()
#define STI()	sti(interruptBackup)
#else

#define EX_INTERRUPT_HANDLER(_SG)
#define CLI()
#define STI()

#endif

#include "diagnostics.h"
#include "communication.h"


//--------------------------------------------------------------------------------------------------------
// Prototypes
//--------------------------------------------------------------------------------------------------------

// INITIALIZE.C	//----------------------------------------------------------------------------------------

void Init_Flags(void);
void Audio_Reset(void);
void Init_Sport0(void);
void Init_UART(void);
void Init_DMA(void);
void Init_Interrupts(void);
void Enable_DMA_Sport0(void);
void Init_Device(void);						// the previous functions aggregated

void enableAudio(void);
void disableAudio(void);

// INTERRUPTS.C	//----------------------------------------------------------------------------------------

EX_INTERRUPT_HANDLER(Sport0_RX_ISR);
EX_INTERRUPT_HANDLER(Uart0_RX_ISR);


// ECOMPLEX.C	//----------------------------------------------------------------------------------------

real cabs2(complex c);
complex cmul2(real a, complex b);
complex cadd2(real a, complex b);
complex csub2(real a, complex b);
complex csqrt(complex c);

uint cisnull(complex  c);
uint cisreal(complex  c);
uint cisimag(complex  c);
uint cisequal(complex  a, complex b);


// FILTERS.C	//----------------------------------------------------------------------------------------

void fPassThrough(void);
void fDirect1(void);


// IMPLEMENT.C	//----------------------------------------------------------------------------------------

int createReferentFilter( filterInfo *fi );

// PZK_CONTAINER.C	//------------------------------------------------------------------------------------

pzkContainer * createPzkContainer(uint np, uint nz);
uint shrinkPzkContainer(pzkContainer * pzk, uint min);
void deletePzkContainer(pzkContainer * pzk);
void sortPzkContainer(pzkContainer * pzk);
void sortPZ(complex * list, uint num);

uint addPole(pzkContainer * pzk, complex pole);
uint addZero(pzkContainer * pzk, complex zero);

uint countPoles(pzkContainer * pzk);
uint countZeros(pzkContainer * pzk);
uint countBiquads(pzkContainer * pzk);


// GENERATE_IIR.C	//------------------------------------------------------------------------------------

int createReferentFilter( filterInfo *fi );

pzkContainer * createButterworth(uint n, real e0);
pzkContainer * createChebyshev1(uint n, real e0);
pzkContainer * createChebyshev2(uint n, real Os, real d2);

iirParameters newIirParameters(void);
int normalizeIirParameters(iirParameters *ip);

int convertParametersForButterworth( iirParameters * ip );
int convertParametersForChebyshev1( iirParameters * ip );
int convertParametersForChebyshev2( iirParameters * ip );


// TRANSFORM.C	//----------------------------------------------------------------------------------------

pzkContainer * t2lp(pzkContainer * pzk, real w0);
pzkContainer * t2hp(pzkContainer * pzk, real w0);
pzkContainer * t2bp(pzkContainer * pzk, real w0, real dw);
pzkContainer * t2bs(pzkContainer * pzk, real w0, real dw);

transformParameters newTransformParameters(void);
int normalizeTransformParameters(transformParameters *tp);
int transformFilter( filterInfo *fi );


// BILINEAR.C	//----------------------------------------------------------------------------------------

int digitalizeFilter( filterInfo *fi );
pzkContainer * bilinear(pzkContainer * pzk, real pwf);
real getPrewarpFactor(real radps);
complex prewarpPZ(complex pz);


// UART.C	//--------------------------------------------------------------------------------------------
uint toggleEcho(void);
uint sendChar(char c);
uint receiveString(void);


// STATE.C	//--------------------------------------------------------------------------------------------
filterInfo newFilterInfo(void);
filterInfo copyFilterInfo(filterInfo * fi);
void printFilterInfo( filterInfo * fi );

int decodeDigitalizationInput(char code[], filterInfo * fi);
int decodeTransformInput(char code[], filterInfo * fi);
int decodeIirInput(char code[], filterInfo * fi);

int changeState(char code[]);
real implementFilter(filterInfo * fi);

int compare1(complex c1, complex c2);
void sortDigitalPZ(pzkContainer * pzk);
int preSortDigitalZeros(pzkContainer * pzk);
int sortDigitalPoles(pzkContainer * pzk);

fract32 direct1(fract16 input, fract16 * coeffs, fract16 * delays);
fract32 passThrough(fract16 input, fract16 * coeffs, fract16 * delays);

#endif

