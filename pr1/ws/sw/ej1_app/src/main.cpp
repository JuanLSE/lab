#include "xil_types.h"
#include "xil_printf.h"
#include "Flexfft.h"
#include "DataIn.h"
#include "xparameters.h"
#include "xparameters_ps.h"
#include "xscutimer.h"
#include "common.hpp"

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
        
    return 0;
}