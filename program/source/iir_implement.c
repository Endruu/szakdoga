#include "../headers/pzk_container.h"		// pzk functions
#include "../headers/diagnostics.h"			// for setting memory usage
#include "../headers/iir_filters.h"			// filtering functions
#include "../headers/iir_functions.h"		// function prototypes
#include "../headers/emath.h"				// math functions
#include "../headers/type_converters.h"		// as name says

/*
#ifndef _COMPILE_WITH_BLACKFIN
#define cabs cabs_custom
#endif
*/
#define MAX_FR16	0x7FFF
#define m1pln2		-1.442695041

real getInfNormScale(complex * z1, complex * z2, complex * p1, complex * p2) {
	real op1, op2, oz1, oz2, tmp, K = 1;
	
	if( p2 == NULL ) {
		op1 = op2 = cabs(cdiv(csub2(1,*p1),csub2(-1,*p1)));
		K = (1-2*p1->re+cabs2(*p1))/4;
	} else {
		op1 = ( p1->re - 1 )/( p1->re + 1 );
		op2 = ( p2->re - 1 )/( p2->re + 1 );
		if( op1 > op2 ) {
			tmp = op1;
			op1 = op2;
			op2 = tmp;
		}
		if( fabs(p1->re) != 1.0 ) {
			K *=( p1->re - 1 )/2;
		}
		if( fabs(p2->re) != 1.0 ) {
			K *=( p2->re - 1 )/2;
		}
	}
	
	if( z2 == NULL ) {
		oz1 = oz2 = cabs(cdiv(csub2(1,*z1),csub2(-1,*z1)));
		K /= 1-2*z1->re+cabs2(*z1)/4;
	} else {
		oz1 = ( z1->re - 1 )/( z1->re + 1 );
		oz2 = ( z2->re - 1 )/( z2->re + 1 );
		if( oz1 > oz2 ) {
			tmp = oz1;
			oz1 = oz2;
			oz2 = tmp;
		}
		if( fabs(z1->re) != 1.0 ) {
			K /=( z1->re - 1)/2;
		}
		if( fabs(z2->re) != 1.0 ) {
			K /=( z2->re - 1)/2;
		}
	}
	
	if( z2 == NULL ) {
		tmp = K*cabs2(csub2(-1, *z1));
	} else {
		tmp = K;
		if( z1->re != -1 ) tmp *= (1+z1->re);
		if( z2->re != -1 ) tmp *= (1+z2->re);
	}
	
	if( p2 == NULL ) {
		tmp /= cabs2(csub2(-1, *p1));
		
		if( tmp > 1.0 ) {
			
			return K/tmp;
		} else {
			
			return K;
		}
			
	} else {
		if( (op1<=oz1 && oz2<=op2) || (oz2<=op1) || (oz1<=op1 && op1<=oz2 && oz2<=op2) ) {
			
			if( p1->re != -1 ) tmp /= (1+p1->re);
			if( p2->re != -1 ) tmp /= (1+p2->re);
			
			if( tmp > 1.0 ) {
				return K/tmp;
			} else {
				return K;
			}
		} else if( oz1<=op1 && op2<=oz2 ) {
			//kell!
		}
	}
	
}

real direct1_biquad( fract16 coeffs[], complex * z1, complex * z2, complex * p1, complex * p2, int scale_mod ) {
	real K = 1, tmp1, tmp2, tmp3, tmp4;
	int sign = 1;
	
	if( !cisreal(*p1) ) {
		if( p2 != p1 && p2 != NULL ) {
			error(1);									// if p1 is complex then p2 is assumed to be its conjugate
		}
		coeffs[4] = float_to_fr16( (float)( -cabs2(*p1) ) );
		coeffs[5] = float_to_fr16( (float)( -p1->re ) );
		
		K *= (1-2*p1->re+cabs2(*p1))/4;
		
	} else {
		tmp1 = -p1->re * p2->re;
		tmp2 = -(p1->re + p2->re)/2;
		
		if( fabs(p1->re) != 1.0 ) {
			K *=( p1->re - 1 )/2;
		}
		if( fabs(p2->re) != 1.0 ) {
			K *=( p2->re - 1 )/2;
		}
		
		if( tmp1 == 1 ) {								// if -p1*p2 = 1	: fract16 can only store [-1,1)
			warn(1);
			if( 1 )	{									// abs p1 and p2 both decreased minimally
				coeffs[4] = MAX_FR16;
				coeffs[5] = 0;
			} else {
				coeffs[4] = MAX_FR16 - 1;
				if( 1 ) {								// pole in 0 is decreased, pole in infinity is maintained
					coeffs[5] = 1;
				} else {								// pole in infinity is decreased, pole in 0 is maintained
					coeffs[5] = -1;
				}
			}
		} else {
			if( tmp2 == 1 ) {							// if p1 = p2 = -1	: fract16 can only store [-1,1)
				warn(2);
				coeffs[5] = MAX_FR16;
				if( 1 )	{
					coeffs[4] = MAX_FR16 - 1;			// p1 decreased, p2 maintained
				}										// else: abs(p1)=abs(p2) p1 = conj(p2)
			} else {
				coeffs[4] = float_to_fr16( (float)tmp1 );
				coeffs[5] = float_to_fr16( (float)tmp2 );
			}
		}
	}
	
	if( !cisreal(*z1) ) {
		if( z2 != z1 && z2 != NULL ) {
			error(2);									// if z1 is complex then z2 is assumed to be its conjugate
		}

		K /= 1-2*z1->re+cabs2(*z1)/4;

		if( K < 0 ) {
			K = -K;
			sign = -1;
		}

		tmp3 = -K*cabs2(*z1);
		tmp2 = K*z1->re;
	} else {
		if( fabs(z1->re) != 1.0 ) {
			K /=( z1->re - 1)/2;
		}
		if( fabs(z2->re) != 1.0 ) {
			K /=( z2->re - 1)/2;
		}

		if( K < 0 ) {
			K = -K;
			sign = -1;
		}
		
		tmp3 = -K * z1->re * z2->re;
		tmp2 = K *( z1->re + z2->re )/2;
	}
	
	tmp1 = -K;
	if( tmp2 < tmp1 || tmp1 == 0 ) tmp1 = tmp2;
	if( tmp3 < tmp1 || tmp1 == 0 ) tmp1 = tmp3;				// tmp1 = min
	tmp4 = tmp3;
	if( tmp2 > tmp4 || tmp4 == 0 ) tmp4 = tmp2;
	if(  -K  > tmp4 || tmp4 == 0 ) tmp4 =  -K ;				// tmp4 = max
	
	tmp1 = floor( log(fabs(tmp1)) * m1pln2 );
	tmp4 = ceil( log(fabs(tmp4)) * m1pln2 );
	if( tmp4 > tmp1 ) tmp4 = tmp1;							// min(tmp1, tmp4)
	tmp1 = pow(2, tmp4);
	tmp3 *= tmp1;
	tmp2 *= tmp1;
	tmp1 *= -K;
	
	coeffs[2] = float_to_fr16( (float)tmp1 );
	coeffs[1] = float_to_fr16( (float)tmp2 );
	coeffs[0] = float_to_fr16( (float)tmp3 );
	coeffs[3] = (int)(-tmp4);
	
	if( sign == 1 ) {
		return K;
	} else {
		return -K;
	}
}

real implementFilter( filterInfo * fi, pzkContainer * digitalized ) {
	int i = 0, j = 0, k = 1;
	complex *p1, *p2, *z1, *z2;
	complex zero;
	real tmp;
	real K = digitalized->amp;
	
	if(countZeros(digitalized) != countPoles(digitalized)) {
		error(3);
	}

	zero.re = 0;
	zero.im = 0;
	
	coeffLines[tmpFilter][0] = countBiquads(digitalized);
	
	sortDigitalPZ(digitalized);
	while( i < digitalized->nextPole ) {
		if( k+6 > COEFF_SIZE ) {
			error(4);
		}
		p1 = &digitalized->poles[i];
		if( cisreal( *p1 ) ) {
			if( i+1 < digitalized->nextPole ) {
				p2 = &digitalized->poles[i+1];
			} else {
				p2 = &zero;
			}
			i += 2;
		} else {
			p2 = NULL;
			i++;
		}
		z1 = &digitalized->zeros[j];
		if( cisreal( *z1 ) ) {
			if( j+1 < digitalized->nextZero ) {
				z2 = &digitalized->zeros[j+1];
			} else {
				z2 = &zero;
			}
			j += 2;
		} else {
			z2 = NULL;
			j++;
		}
		tmp = direct1_biquad(coeffLines[tmpFilter]+k, z1, z2, p1, p2, 0);
		if(tmp) {
			K /= tmp;
		} else {
			error(5);
		}
		k += 6;
	}
	
	if( k+1 > COEFF_SIZE ) {
		error(6);
	}
	
	coeffLines[tmpFilter][k] = float_to_fr16( (float)K );
	
	fi->filter = &direct1;
	
	//printf("fK = %g\n", K);
	return K;
}
