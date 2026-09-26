#include "xil_printf.h"
#include "DataIn.h"
#include "Flexfft.h"
#include "xparameters.h"
int main(){
    xil_printf("Hola mundo\n");


    sampleOutX_t DataOut[FFT_LENGTH];
    FlexFFT(DataIn, DataOut);
    

    
    return 0;
}