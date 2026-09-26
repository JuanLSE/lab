#include "xil_printf.h"
#include "Flexfft.h"
#include "DataIn.h"
#include "xparameters.h"
#include "xparameters_ps.h"
#include "xscutimer.h"

#define TIMER_DEVICE_BASEADDR XPAR_XSCUTIMER_0_BASEADDR
#define TIMER_LOAD_VALUE 0xFFFFFFFF
#define TIMER_FREQ (XPAR_CPU_CORTEXA9_CORE_CLOCK_FREQ_HZ/ 2U)

XScuTimer TimerInstance;

int main(){
    int Status;
    sampleOutX_t DataOut[FFT_LENGTH];
    xil_printf("Hola mundo\n");

    XScuTimer_Config *ConfigPtr;
    ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_BASEADDR);
    if (ConfigPtr == NULL) { xil_printf("LookupConfig failed\n"); return XST_FAILURE; }


    Status = XScuTimer_CfgInitialize(&TimerInstance, ConfigPtr, TIMER_DEVICE_BASEADDR);
	if (Status != XST_SUCCESS) {
        xil_printf("Timer init failed: %d\n", Status);
		return XST_FAILURE;
	}

    //XScuTimer_EnableAutoReload(&TimerInstance);
    XScuTimer_DisableAutoReload(&TimerInstance);
    XScuTimer_LoadTimer(&TimerInstance,TIMER_LOAD_VALUE);

    XScuTimer_Start(&TimerInstance);
    FlexFFT(DataIn, DataOut);
    XScuTimer_Stop(&TimerInstance);

    u32 loop_ticks = TIMER_LOAD_VALUE - XScuTimer_GetCounterValue(&TimerInstance);
    u32 sec_interval = loop_ticks/TIMER_FREQ;
    xil_printf("FlexFFT took: %lu ticks\nInterval in seconds: %lu", 
        loop_ticks,
        sec_interval);
    
    
    return 0;
}