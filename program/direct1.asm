/*
	fract16 direct1(fract16 * coeffs, fract16 * delays, int shifts, fract16 input );
*/

.section L1_code;

.global _direct1;
_direct1:

	P0 = R0;			// Start address of the coeffs
	I0 = R1;			// Start address of the coeffs

	R1.H = W[P0++]  || R1.L = W[I0++];
	
	A0 = R1.H * R3.L || R1.H = W[P0++] ;				// +b0*x[k]
	A0 += R1.H * R1.L;								// +b1/2*x[k-1]
	A0 += R1.H * R1.L || R1.H = W[P0++]  || R1.L = W[I0++];	// +b1/2*x[k-1]
	A0 += R1.H * R1.L || R1.H = W[P0++]  || R1.L = W[I0++];	// +b2*x[k-2]
	A0 = ASHIFT A0 BY R2.L;							// = K( b0*x[k] + b1*x[k-1] + b2*x[k-2] )
	A0 += R1.H * R1.L;								// +a1/2*y[k-1]
	A0 += R1.H * R1.L || R1.H = W[P0++]  || R1.L = W[I0++];	// +a1/2*y[k-1]
	A0 += R1.H * R1.L;								// +a2*y[k-2]

	A0 = A0 (S);
	R0.l = A0;
	RTS;

_direct1.end: