#ifndef __SOFTWARE_PWM_H_
#define __SOFTWARE_PWM_H_

#include <stdint.h>

/*软件PWM设备个数*/
#define SOFT_PWM_MAX_NUM 10 

typedef struct __soft_pwm_device
{
    uint32_t Base;
    uint32_t Pin;
    uint32_t current_cnt;
    uint32_t cnt_high;
    uint32_t full_width_cnt;//周期全长:是TIM_ITV_US的多少倍
}_soft_pwm_device;

extern _soft_pwm_device* soft_pwm_device_chain[SOFT_PWM_MAX_NUM];

_soft_pwm_device* Soft_PWM_Add(uint32_t Base,uint32_t Pin,uint32_t full_width_us);
void Set_Duty_Soft(_soft_pwm_device* device,float duty);
void Soft_PWM_Solve(void);


#endif
