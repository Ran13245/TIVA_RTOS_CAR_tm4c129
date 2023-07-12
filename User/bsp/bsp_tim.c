#include <stdint.h>
#include <stdbool.h>
#include "bsp_tim.h"
#include "hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/timer.h"
#include "tm4c129_it.h"
#include "config.h"

void init_drv_tim(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
    TimerConfigure(TIMER1_BASE,TIMER_CFG_PERIODIC);
    TimerClockSourceSet(TIMER1_BASE,TIMER_CLOCK_SYSTEM);
    TimerLoadSet(TIMER1_BASE,TIMER_A,1e-6*TIM_ITV_US*USER_SYS_FREQ);
    TimerIntRegister(TIMER1_BASE,TIMER_A,TIM1_IRQHandler);
    TimerIntEnable(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
    TimerEnable(TIMER1_BASE,TIMER_A);
}

