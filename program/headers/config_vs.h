// Input buffersize
#define INPUT_BUF_SIZE				1000

// Placement in memory
#define DELAY_SECTION
#define COEFF_SECTION

// parameters for preallocated data/coefficient arrays
#define COEFF_SIZE					100
#define DELAY_SIZE					100
#define COEFF_TYPE					fract16
#define DELAY_TYPE					fract16

// filter input/output types
#define INPUT_TYPE					fract16
#define OUTPUT_TYPE					fract32
#define INPUT_MAX					0x7FFF

// sampling rate
#define F_SAMPLING					48000.0

// Number of filters
#define FILTER_NUM					2

// Debugging
#define	DEBUG_FUNCTIONS_ENABLED
// Testcase
#define TESTCASE_DIR				"../simulation/testcase/"
#define	TC_CALL_MAX_DEPTH			3
#define COPY_COMMENTS				2			// 0 - dont, 1 - mark, 2 - copy
// Simulation
#define SIM_INPUT_DIR				"../simulation/input/"
#define SIM_OUTPUT_DIR				"../simulation/output/"

// Logging
#define LOGGING_DIR					"../simulation/log/"
#define LOGGING_ENABLED
#define LOGGING_MODE				"w"			// "a" - append, "w" - write

// Error
#define ERROR_BUFFER_SIZE			30

// Diagnostics
//#define DISABLE_DIAGNOSTICS
#define ENABLE_DIAG_CU				// cpu usage
#define ENABLE_DIAG_MU				// memory usage
#define ENABLE_DIAG_CW				// codeword
// For CPU usage
#define CLOCK_CALIBRATION_LENGTH	16

// Default e0 values for referent filters
#define DEFAULT_BW_E0				1.0			// 1/sqrt(2) (~3dB) attenuation at corner freq
#define DEFAULT_C1_E0				0.5			// ~1dB passband ripple
#define DEFAULT_C2_E0				1.0			// 1/sqrt(2) (~3dB) attenuation at corner freq

// Limits for the filters
#define MAX_DB_INPUT				240
#define MIN_LIN_INPUT				1e-12
#define MAX_WS_INPUT				1200.0		// usable freq range: 20-24000Hz
#define MIN_BANDWIDTH				100			// ~16Hz hasraütve
#define MIN_FREQUENCY				0.001		// ~20Hz
#define MAX_FREQUENCY				150000		// ~24kHz