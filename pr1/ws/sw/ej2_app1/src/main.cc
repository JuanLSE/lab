/*
 * Empty C++ Application
 */
#include <stdio.h>
#include "xparameters.h"
#include "xil_printf.h"
#include <complex>

#include "xparameters.h"
#include "xparameters_ps.h"
#include "xscutimer.h"
#include "common.hpp"

using namespace std;
typedef std::complex<float> sampleInX_t;
typedef std::complex<float> sampleOutX_t;

#define FFT_LENGTH 512

#include "DataIn.h"
#include "DataOut_OK.h"
#include "Flexfft.h"
// NEON libraries
#include "Ne10.h"

// Timer libraries
//#include "xtime_l.h"

// Number of FFT points
#define NFFT FFT_LENGTH

// Number of iterations
#define TEST_SAMPLES	1000

XScuTimer TimerInstance;

int main()
{
	printf("*****************************************************\n");
	printf("***  Launching the FFT test program (%d) ***\n", NFFT);
	printf("******************************************************\n");

    if (init_timer(&TimerInstance, TIMER_DEVICE_BASEADDR) != XST_SUCCESS) {
        return XST_FAILURE;
    }
	// Size of FFT
	ne10_int32_t fftSize = NFFT;

	// Pointer of src_input FFT
	ne10_fft_cpx_float32_t *psrc_ddr; // = XPAR_DDR_MEM_BASEADDR; // Its posible to set memory baseaddress
	// Pointer of dst_output FFT
	ne10_fft_cpx_float32_t *pdst_ddr; // = XPAR_DDR_MEM_BASEADDR + 0x8000;
	// Configuration variable FFT
	ne10_fft_cfg_float32_t cfg;

	// Variables to generate input and read output
	uint32_t i;


	// Checking if NEON is avilable
	if (ne10_init() != NE10_OK)
	{
		xil_printf("\nFailed to initialise Ne10.\n");
		return 1;
	}

	// printf("***  Alloc memory to FFT test program  ***\n");
	// printf("***********************************************\n");
	// Configuration of pointers and coefficients of FFT.
	cfg = ne10_fft_alloc_c2c_float32_neon(fftSize);
	// Initialization of pointers src and dst
	psrc_ddr = (ne10_fft_cpx_float32_t *) (cfg->buffer + (sizeof (ne10_fft_cpx_float32_t) * (fftSize)));
	pdst_ddr = (ne10_fft_cpx_float32_t *) (psrc_ddr + (sizeof (ne10_fft_cpx_float32_t) * (fftSize)));

	// Check if we can remove this part
	if (cfg == NULL)
	{
		xil_printf("\nERROR! Alloc failed.\n");
		return -1;
	}


	// printf("***  Generating the input to FFT test program  ***\n");
	// printf("*******************************************************\n");
	for (i = 0; i < NFFT; i++)
	{
		//psrc_ddr[i] = (ne10_fft_cpx_float32_t) DataIn[i];
		psrc_ddr[i].r = DataIn[i].real();
		psrc_ddr[i].i = DataIn[i].imag();
		//psrc_ddr[i]= {DataIn[i].real(), DataIn[i].imag()};
	}
	

	// printf("***  Launch %d iters of FFT test program  ***\n", TEST_SAMPLES);
	// printf("********************************************************\n");
	XScuTimer_Start(&TimerInstance);

	for (int m = 0; m < TEST_SAMPLES; m++)
	{
		ne10_fft_c2c_1d_float32(pdst_ddr, psrc_ddr, cfg, 0);
	};
    XScuTimer_Stop(&TimerInstance);


	// printf("***  Time of execution of FFT test program  ***\n");
	// printf("********************************************************\n");

	//printf("Time average of execution: %.4f us.\n", time_avg / TEST_SAMPLES);
    u32 load       = XScuTimer_GetLoadReg(TIMER_DEVICE_BASEADDR);
    u32 loop_ticks = load - XScuTimer_GetCounterValue(&TimerInstance);
    u32 us_interval = (u32)(((u64)loop_ticks * 1000000ULL) / (u64)TIMER_FREQ);
    u32 avg_ticks  = loop_ticks / TEST_SAMPLES;
    u32 avg_us     = us_interval / TEST_SAMPLES;

    xil_printf("FlexFFT total: %u ticks\nInterval total: %u ms %u us\n"
               "Average per iteration: %u ticks / %u us\n",
        (unsigned int)loop_ticks,
        (unsigned int)(us_interval / 1000U),
        (unsigned int)(us_interval % 1000U),
        (unsigned int)avg_ticks,
        (unsigned int)avg_us);


	// printf("***  Validation with the golden pattern  ***\n");
	// printf("********************************************************\n");
	//....................................



 //average error:
	float total_real_error = 0.0f;
	float total_imag_error = 0.0f;
	for (i = 0; i < NFFT; i++)
	{
		const float real_error = fabsf(pdst_ddr[i].r - DataOut_OK[i].real());
		const float imag_error = fabsf(pdst_ddr[i].i - DataOut_OK[i].imag());
		total_real_error += real_error;
		total_imag_error += imag_error;
	}
	const float avg_real_error = total_real_error / NFFT;
	const float avg_imag_error = total_imag_error / NFFT;
	printf("Average real error: %f\n", avg_real_error);
	printf("Average imag error: %f\n", avg_imag_error);
 

	printf("***********************************************\n");
	printf("***  End of FFT test program  ***\n");
	printf("***********************************************\n");


	return 0;
}
