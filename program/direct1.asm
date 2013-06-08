/*
	fract32 direct1(fract16 * coeffs, fract16 * delays, unsigned int count);
*/

.section L1_code;

.global _direct1;
_direct1:

	// init:
	I0 = R0;												// address of next coeff
	I1 = R1;												// address of next delay
	LC0 = R2;												// counter
	
															// R2.L : next coefficient
															// R1.L : next delayed value
															// R0.L : x[k]														
	R0.L = W[I1++];											// input: x[k] = delays[0]
	I3 = I1;												// address of previous delay
	R2.L = W[I0++] || R1.L = W[I1++];						// load: b2, x[k-2]
	
	LOOP main_loop LC0;										// loop setup
	LOOP_BEGIN main_loop;
	A0 = 0;													// accumulator
	A0 -= R2.L * R1.L || R2.L = W[I0++] || R1.L = W[I1++];	// b2*x[k-2]		- load: b1, x[k-1]
	W[I3++] = R1.L;											// x[k-2] = x[k-1]
	A0 -= R2.L * R1.L;										// b1/2*x[k-1]
	A0 -= R2.L * R1.L || R2.L = W[I0++] || R1.L = W[I1++];	// b1/2*x[k-1]		- load: b0, y[k-2]
	W[I3++] = R0.L;											// x[k-1] = x[k]
	A0 -= R2.L * R0.L || R2.L = W[I0++];					// b0*x[k]
		
	A0 = ASHIFT A0 BY R2.L || R2.L = W[I0++];				// = K( b0*x[k] + b1*x[k-1] + b2*x[k-2] ) - load: a2
	
	A0 += R2.L * R1.L || R2.L = W[I0++] || R1.L = W[I1++];	// a2*y[k-2]		- load: a0, y[k-1]
	W[I3++] = R1.L;											// y[k-2] = y[k-1]
	A0 -= R2.L * R1.L;										// a1/2*y[k-1]
	A0 -= R2.L * R1.L || R2.L = W[I0++] || R1.L = W[I1++];	// a1/2*y[k-1]		- load: next b2, x[k-2]
	R0.L = A0;												// acc(9.31) => R0.L(1.15) = output: y[k]
	W[I3++] = R0.L;											// y[k-1] = y[k]
	LOOP_END main_loop;
	
	R0 = A0;												// full 1.31 output
	RTS;

_direct1.end: