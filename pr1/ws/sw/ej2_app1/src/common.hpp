#ifndef COMMON_HPP
#define COMMON_HPP

#include "xscutimer.h"

#define TIMER_DEVICE_BASEADDR XPAR_XSCUTIMER_0_BASEADDR
#define TIMER_LOAD_VALUE 0xFFFFFFFF
#define TIMER_FREQ (XPAR_CPU_CORE_CLOCK_FREQ_HZ / 2U)

int init_timer(XScuTimer *timer, u32 base_addr);

#endif
