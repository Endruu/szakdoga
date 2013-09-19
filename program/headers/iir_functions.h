#ifndef  _IIR_FUNCTIONS_H 
#define _IIR_FUNCTIONS_H 

#include "variables.h"
#include "pzk_container.h"
#include "emath.h"

// Generator

int parseIirDigitalizationParameters( char * s, int l, filterInfo * fi );
int parseIirReferentParameters( char * s, int l, filterInfo * fi );
int parseIirTransformParameters( char * s, int l, filterInfo * fi );

int cmdGenerateIir( char * s, int l, int modify );

// Referent

pzkContainer * createReferentFilter( filterInfo *fi );

pzkContainer * createButterworth(uint n, real e0);
pzkContainer * createChebyshev1(uint n, real e0);
pzkContainer * createChebyshev2(uint n, real Os, real d2);

int normalizeIirParameters(iirParameters *ip);

int convertParametersForButterworth( iirParameters * ip );
int convertParametersForChebyshev1( iirParameters * ip );
int convertParametersForChebyshev2( iirParameters * ip );

// Transform

pzkContainer * transformFilter( filterInfo *fi, pzkContainer * referent );

pzkContainer * t2lp(pzkContainer * pzk, real w0);
pzkContainer * t2hp(pzkContainer * pzk, real w0);
pzkContainer * t2bp(pzkContainer * pzk, real w0, real dw);
pzkContainer * t2bs(pzkContainer * pzk, real w0, real dw);

int normalizeTransformParameters(transformParameters *tp);

// Digitalize

pzkContainer * digitalizeFilter( filterInfo *fi, pzkContainer * transformed );

pzkContainer * bilinear(pzkContainer * pzk, real pwf);
real getPrewarpFactor(real radps);
complex prewarpPZ(complex pz);

// Implement

real getInfNormScale(complex * z1, complex * z2, complex * p1, complex * p2);
real direct1_biquad( fract16 coeffs[], complex * z1, complex * z2, complex * p1, complex * p2, int scale_mod );
real implementFilter( filterInfo * fi, pzkContainer * digitalized );

#endif
