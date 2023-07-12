#include <stdbool.h>

#include "software_pwm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "config.h"

#include "FreeRTOS.h"
#include "portable.h"

_soft_pwm_device* soft_pwm_device_chain[SOFT_PWM_MAX_NUM];

uint8_t device_num;

/**
 * @brief 添加软件PWM设备
 * 
 * @param Base 输出PWM BASE
 * @param Pin 输出PWM PIN
 * @param full_width_us 全周期脉宽长度,us
 * @return _soft_pwm_device* 设备地址 
 */
_soft_pwm_device* Soft_PWM_Add(uint32_t Base,uint32_t Pin,uint32_t full_width_us){
    if(device_num!=SOFT_PWM_MAX_NUM){
        GPIOPinTypeGPIOOutput(Base, Pin);
        _soft_pwm_device* new_device=pvPortMalloc(sizeof(_soft_pwm_device));
        new_device->Base=Base;
        new_device->Pin=Pin;
        new_device->full_width_cnt=full_width_us/TIM_ITV_US;
        new_device->cnt_high=0;
        new_device->current_cnt=0;

        soft_pwm_device_chain[device_num]=new_device;
        device_num++;

        return new_device;
    }
    else {
        return NULL;
    }
}

void Set_Duty_Soft(_soft_pwm_device* device,float duty){
    if(duty >1.0F) duty=1.0F;
    if(duty < 0.0F) duty=0.0F;
    device->cnt_high=device->full_width_cnt*duty;
}

void Soft_PWM_Solve(void){
    uint8_t i;
    _soft_pwm_device* dev=NULL;
    for(i=0;i<device_num;i++){
        dev=soft_pwm_device_chain[i];
        if(dev->current_cnt < dev->cnt_high){
            GPIOPinWrite(dev->Base,dev->Pin,dev->Pin);
        }
        else{
            GPIOPinWrite(dev->Base,dev->Pin,~dev->Pin);
        }
        dev->current_cnt=(dev->current_cnt+1)%dev->full_width_cnt;
    }
}


