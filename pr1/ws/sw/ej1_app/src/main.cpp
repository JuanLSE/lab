#include "xil_printf.h"
#include "Flexfft.h"
#include "DataIn.h"
#include "xparameters.h"
#include "xscutimer.h"

#define TIMER_DEVICE_BASEADDR XPAR_XSCUTIMER_0_BASEADDR
#define TIMER_LOAD_VALUE 0xFFFF

XScuTimer TimerInstance;

int main(){
    int Status;
    xil_printf("Hola mundo\n");

    XScuTimer_Config *ConfigPtr;
    ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_BASEADDR);

    XScuTimer_CfgInitialize(&TimerInstance, ConfigPtr, ConfigPtr)
	if (Status != XST_SUCCESS) {
        xil_printf("No se pudo inicializar el la configuración del timer\n");
		return XST_FAILURE;
	}

    XScuTimer_EnableAutoReload(&TimerInstance);

    XScuTimer_LoadTimer(&TimerInstance,TIMER_LOAD_VALUE);
    

    sampleOutX_t DataOut[FFT_LENGTH];
    FlexFFT(DataIn, DataOut);
    

    
    return 0;
}