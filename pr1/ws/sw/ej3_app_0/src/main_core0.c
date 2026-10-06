#include "xparameters.h"
#include "xil_mmu.h"
#include "xparameters_ps.h"
#include "xscutimer.h"
#include "common.hpp"
#include "platform.h"
#include "mutex_blk.h"

#undef SHAREABLE
#define SHAREABLE (1 << 16)
#define NON_CACHED (0)
#define WB_WA_CACHE (1) /* write back, write allocate */
#define WT_CACHE (2) /* write thru */
#define WB_CACHE (3) /* write back */
#define INNER_SHIFT (2)
#define OUTER_SHIFT (12)
#define TEX_MSB (1 << 14)
#define AP_FAULT (0) /* no access, a fault */
#define AP_FULL (3 << 10) /* full access */
#define DOMAIN(x) (x << 5)
#define INNER_NON_CACHED (NON_CACHED << INNER_SHIFT)
#define INNER_WB_WA (WB_WA_CACHE<< INNER_SHIFT)
#define INNER_WT (WT_CACHE<< INNER_SHIFT)
#define INNER_WB (WB_CACHE<< INNER_SHIFT)
#define OUTER_NON_CACHED (TEX_MSB | (NON_CACHED << OUTER_SHIFT))
#define OUTER_WB_WA (TEX_MSB | (WB_WA_CACHE<< OUTER_SHIFT))
#define OUTER_WT (TEX_MSB | (WT_CACHE<< OUTER_SHIFT))
#define OUTER_WB (TEX_MSB | (WB_CACHE << OUTER_SHIFT))
#define SECTION(attributes) ((attributes) | 0x2)
#define SUPER_SECTION(attributes) ((attributes) | 0x4002)

// Las optimizaciones del compilador pueden hacer que optimice la localizaci�n de estas variables dentro del mapa de memoria. Para que en ambos procesadores est�n en la misma regi�n, se estable la m�nima optimizaci�n (o0) en esta parte del programa en ambos cores.
#pragma GCC push_options
#pragma GCC optimize ("O0")

// Mutex en cada core que se aloja en la secci�n .mem_mutex del linker script con alineamiento d 32 bits.
volatile unsigned int ctrl_mutex __attribute__ ((section (".mem_mutex"))) __attribute__( ( aligned ( 32 ) ) ) = unlocked;

#define DATA_LEN 100
volatile unsigned int data[DATA_LEN] __attribute__ ((section (".mem_mutex"))) __attribute__( ( aligned ( 32 ) ) );
volatile unsigned int result[DATA_LEN] __attribute__ ((section (".mem_mutex"))) __attribute__( ( aligned ( 32 ) ) );

#pragma GCC pop_options

XScuTimer TimerInstance;

int main()
{
init_platform();
Xil_SetTlbAttributes(0x03000000, SECTION(SHAREABLE|NON_CACHED));
int bandera=0;
int i=0;
int N=0;
// TO DO

xil_printf("\rComienzo del programa");

    if (init_timer(&TimerInstance, TIMER_DEVICE_BASEADDR) != XST_SUCCESS) {
        return XST_FAILURE;
    }

XScuTimer_Start(&TimerInstance);

lock_mutex((void*)&ctrl_mutex);

for(i=0;i<DATA_LEN;i++){
data[i]=i;
}
//Ahora hacemos el cálculo
for(N=0;N<DATA_LEN;N++){
result[N]=7*data[N];
}
//Liberamos el mutex
unlock_mutex((void*)&ctrl_mutex);
//Ahora paramos la medida de tiempo
XScuTimer_Stop(&TimerInstance);
//Ahora se imprime el tiempo medido
    u32 load       = XScuTimer_GetLoadReg(TIMER_DEVICE_BASEADDR);
    u32 loop_ticks = load - XScuTimer_GetCounterValue(&TimerInstance);
    u32 us_interval = (u32)(((u64)loop_ticks * 1000000ULL) / (u64)TIMER_FREQ);
    xil_printf("FlexFFT1 took: %u ticks\nInterval: %u ms %u us\n",
        (unsigned int)loop_ticks,
        (unsigned int)(us_interval / 1000U),
        (unsigned int)(us_interval % 1000U));

        
XScuTimer_Start(&TimerInstance);

lock_mutex((void*)&ctrl_mutex);

for(i=0;i<DATA_LEN;i++){
data[i]=i;
}

unlock_mutex((void*)&ctrl_mutex);

for(N=0;N<DATA_LEN/2;N++){
result[N]=7*data[N];
}
while (bandera==0){
}

XScuTimer_Stop(&TimerInstance);

    load       = XScuTimer_GetLoadReg(TIMER_DEVICE_BASEADDR);
    loop_ticks = load - XScuTimer_GetCounterValue(&TimerInstance);
    us_interval = (u32)(((u64)loop_ticks * 1000000ULL) / (u64)TIMER_FREQ);
    xil_printf("FlexFFT took: %u ticks\nInterval: %u ms %u us\n",
        (unsigned int)loop_ticks,
        (unsigned int)(us_interval / 1000U),
        (unsigned int)(us_interval % 1000U));
//xil_printf("FFT run biprocessor: %llu clock cycles.", 2(tEnd-tStart));
//xil_printf("FFT run biprocessor: %.4f us.", 1.0(tEnd-tStart)/(COUNTS_PER_SECOND/1000000));
cleanup_platform();
return 0;
}