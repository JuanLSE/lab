#include "common.hpp"
#include "xil_printf.h"
#include "xil_types.h"
#include "xparameters.h"

int init_timer(XScuTimer *timer, u32 base_addr) {
    XScuTimer_Config *ConfigPtr;
    int Status;

    ConfigPtr = XScuTimer_LookupConfig(base_addr);
    if (ConfigPtr == NULL) { xil_printf("LookupConfig failed\n"); return XST_FAILURE; }

    Status = XScuTimer_CfgInitialize(timer, ConfigPtr, base_addr);
    if (Status != XST_SUCCESS) {
        xil_printf("Timer init failed: %d\n", Status);
        return XST_FAILURE;
    }

    //XScuTimer_EnableAutoReload(timer);
    XScuTimer_DisableAutoReload(timer);
    XScuTimer_LoadTimer(timer, TIMER_LOAD_VALUE);

    return XST_SUCCESS;
}
