#include <stdint.h>
#include <stdbool.h>
#include "hw_memmap.h"
#include "driverlib/pwm.h"
#include "bsp_pwm.h"
#include "main.h"
#include "buzzer.h"

#include "FreeRTOS.h"
#include "semphr.h"

extern SemaphoreHandle_t semphr_buzzer_trigger;

/*!
 * @brief 设置蜂鸣器频率
 * @param hz 考虑到脉宽只有16位，应该大于230.设置0表示关闭
 */
void Set_Buzzer_Hz(float hz){
    if(hz!=0.0F){
        PWMGenPeriodSet(PWM0_BASE,PWM_GEN_3,1.0F/hz*PWMCLK_FRQ);
        Set_Duty(PWM_BUZZER_BASE,PWM_BUZZER_OUT,0.5F);
    }
    else{
        Set_Duty(PWM_BUZZER_BASE,PWM_BUZZER_OUT,0);
    }
}

/*!
 * @brief 蜂鸣器运行
 * @param start_time 开始时间，FreeRTOS systick
 * @param current_time 当前时间，FreeRTOS systick
 */
void Buzzer_Buzz(uint32_t start_time, uint32_t current_time){
    float time=(current_time-start_time)*0.001;/*FreeRTOS Systick*/
    static uint32_t last_start_time=0;
    static uint8_t state=0;
    if(start_time!=last_start_time){state=0; last_start_time=start_time;}
    switch (state)
    {
        case 0:{
            Set_Buzzer_Hz(2730.0F);
            if(time > 0.3F){
                state++;
            }
            break;
        }
        case 1:{
            Set_Buzzer_Hz(0);
            if(time > 0.4F){
                state++;
            }
            break;
        }
        case 2:{
            Set_Buzzer_Hz(2730.0F);
            if(time > 0.7F){
                state++;
            }
            break;
        }
        
        default:{
            Set_Buzzer_Hz(0);
            break;
        }
        
    }
}

/*!
 * @brief 触发蜂鸣器
 * @param  
 */
void Buzzer_Trigger(void){
    xSemaphoreGive(semphr_buzzer_trigger);
}
