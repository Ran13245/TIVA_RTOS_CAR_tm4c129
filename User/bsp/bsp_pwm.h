/**
 * @file bsp_pwm.h
 * @author cbr (ran13245@outlook.com)
 * @brief 板级PWM驱动
 * @version 0.1
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __BSP_PWM_H_
#define __BSP_PWM_H_

#include <stdint.h>

void init_drv_PWM(void);
void Set_Duty(uint32_t Base,uint32_t PWMOut,float duty);


#endif
