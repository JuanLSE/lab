#include "xil_types.h"
#include "xil_printf.h"
#include "Flexfft.h"
#include "DataIn.h"
#include "xparameters.h"
#include "xparameters_ps.h"
#include "xscutimer.h"
#include "common.hpp"
#include "DataOut_OK.h"

XScuTimer TimerInstance;

int main(){
    sampleOutX_t DataOut[FFT_LENGTH];
    xil_printf("Hola mundo\n");

    if (init_timer(&TimerInstance, TIMER_DEVICE_BASEADDR) != XST_SUCCESS) {
        return XST_FAILURE;
    }

    XScuTimer_Start(&TimerInstance);
    FlexFFT(DataIn, DataOut);
    XScuTimer_Stop(&TimerInstance);

    u32 load       = XScuTimer_GetLoadReg(TIMER_DEVICE_BASEADDR);
    u32 loop_ticks = load - XScuTimer_GetCounterValue(&TimerInstance);
    u32 us_interval = (u32)(((u64)loop_ticks * 1000000ULL) / (u64)TIMER_FREQ);
    xil_printf("FlexFFT took: %u ticks\nInterval: %u ms %u us\n",
        (unsigned int)loop_ticks,
        (unsigned int)(us_interval / 1000U),
        (unsigned int)(us_interval % 1000U));

    //average error:
    float total_real_error = 0.0f;
    float total_imag_error = 0.0f;
    for (int i = 0; i < FFT_LENGTH; i++)
    {
        // la funcion fabsf calcula el valor absoluto de un float
        const float real_error = fabsf(DataOut[i].real() - DataOut_OK[i].real());
        const float imag_error = fabsf(DataOut[i].imag() - DataOut_OK[i].imag());
        total_real_error += real_error;
        total_imag_error += imag_error;
    }
    const float avg_real_error = total_real_error / FFT_LENGTH;
    const float avg_imag_error = total_imag_error / FFT_LENGTH;
    xil_printf("Average real error: %f\n", avg_real_error);
    xil_printf("Average imag error: %f\n", avg_imag_error);
        
    return 0;
}