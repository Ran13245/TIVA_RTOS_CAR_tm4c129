#include "bsp.h"

void Tim_CallBack(void){
    TimerIntClear(TIMER1_BASE,TIMER_TIMA_TIMEOUT);
    Soft_PWM_Solve();
}
