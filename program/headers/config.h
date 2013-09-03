#ifdef __VISUALDSPVERSION__
#define _COMPILE_WITH_BLACKFIN	// uncomment to compile with other compiler, some functions will be unavailable
#endif

#ifndef  _CONFIG_H
#define _CONFIG_H

// UART buffersize
#define UART_BUF_SIZE			100

// parameters for preallocated data/coefficient arrays
#define COEFF_SIZE				100
#define DELAY_SIZE				100

#define COEFF_TYPE				fract16
#define DELAY_TYPE				fract16


#define DEFAULT_BW_E0			1.0			// 1/sqrt(2) (~3dB) attenuation at corner freq
#define DEFAULT_C1_E0			0.5			// ~1dB passband ripple
#define DEFAULT_C2_E0			1.0			// 1/sqrt(2) (~3dB) attenuation at corner freq

#define MAX_DB_INPUT			240
#define MIN_LIN_INPUT			1e-12
#define MAX_WS_INPUT			1200.0		// usable freq range: 20-24000Hz

#define MIN_BANDWIDTH			100			// ~16Hz hasraütve
#define MIN_FREQUENCY			0.001	//125			// ~20Hz
#define MAX_FREQUENCY			150000		// ~24kHz


#define BAUD_RATE				57600

#define CPU_FREQ				600000000

#endif