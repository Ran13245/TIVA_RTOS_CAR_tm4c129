/**
 * @file bsp_pwm.c
 * @author cbr (ran13245@outlook.com)
 * @brief 板级PWM驱动
 * @version 0.1
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "main.h"
#include "hw_memmap.h"
#include "driverlib/gpio.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include <stdbool.h>
#include <stdint.h>
#include "bsp_pwm.h"

/*!
 * @brief 初始化PWM模块
 * @param  void
 */
void init_drv_PWM(void){
	SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);

	GPIOPinConfigure(GPIO_PF0_M0PWM0);
	GPIOPinConfigure(GPIO_PF1_M0PWM1);
	GPIOPinConfigure(GPIO_PF2_M0PWM2);
	GPIOPinConfigure(GPIO_PF3_M0PWM3);
	GPIOPinConfigure(GPIO_PG0_M0PWM4);
	GPIOPinConfigure(GPIO_PG1_M0PWM5);
	// GPIOPinConfigure(GPIO_PK4_M0PWM6);
	GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
	// GPIOPinTypePWM(GPIO_PORTK_BASE, GPIO_PIN_4);
	GPIOPinTypePWM(GPIO_PORTG_BASE, GPIO_PIN_0 | GPIO_PIN_1);
	
	PWMClockSet(PWM0_BASE,PWM_SYSCLK_DIV_64);
	PWMGenConfigure(PWM0_BASE,PWM_GEN_0,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); // 2 channel of PWM for Left Motors
	PWMGenConfigure(PWM0_BASE,PWM_GEN_1,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); // 2 channel of PWM for Right Motors
	// PWMGenConfigure(PWM0_BASE,PWM_GEN_3,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); // 1 channel of PWM for Buzzer
	PWMGenConfigure(PWM0_BASE,PWM_GEN_2,PWM_GEN_MODE_DOWN | PWM_GEN_MODE_NO_SYNC); // 2 channel of PWM for Servo
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_0,200);
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_1,200);
	PWMGenPeriodSet(PWM0_BASE,PWM_GEN_2,37500);
	// PWMGenPeriodSet(PWM0_BASE,PWM_GEN_3,20000);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_0,0);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_1,0);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_2,0);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_3,0);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_4,0);
	PWMPulseWidthSet(PWM0_BASE,PWM_OUT_5,0);
	// PWMPulseWidthSet(PWM0_BASE,PWM_OUT_6,0);
	PWMGenEnable(PWM0_BASE, PWM_GEN_0);
	PWMGenEnable(PWM0_BASE, PWM_GEN_1);
	PWMGenEnable(PWM0_BASE, PWM_GEN_2);
	// PWMGenEnable(PWM0_BASE, PWM_GEN_3);

    /* Turn on PWM output pins */
	PWMOutputState(PWM0_BASE, PWM_OUT_4_BIT | PWM_OUT_5_BIT, false);
	// PWMOutputState(PWM0_BASE, PWM_OUT_6_BIT, false);
	PWMOutputState(PWM0_BASE, PWM_OUT_0_BIT | PWM_OUT_1_BIT | PWM_OUT_2_BIT | PWM_OUT_3_BIT, false);
}


/*!
 * @brief 函数功能：设置占空比
 * @param Base：PWM模块编号
 * @param PWMOut：PWM信号编号
 * @param duty: PWM占空比
 */
void Set_Duty(uint32_t Base,uint32_t PWMOut,float duty)
{
	if(duty < 0.0F) duty=0.0F;
	if(duty > 1.0F) duty=1.0F;
	uint32_t Gen;
	uint32_t OutBits;
	switch(PWMOut)
	{
		case PWM_OUT_0:	Gen=PWM_GEN_0,OutBits=PWM_OUT_0_BIT;	break;
		case PWM_OUT_1:	Gen=PWM_GEN_0,OutBits=PWM_OUT_1_BIT;	break;
		case PWM_OUT_2:	Gen=PWM_GEN_1,OutBits=PWM_OUT_2_BIT;	break;
		case PWM_OUT_3:	Gen=PWM_GEN_1,OutBits=PWM_OUT_3_BIT;	break;
		case PWM_OUT_4:	Gen=PWM_GEN_2,OutBits=PWM_OUT_4_BIT;	break;
		case PWM_OUT_5:	Gen=PWM_GEN_2,OutBits=PWM_OUT_5_BIT;	break;
		case PWM_OUT_6:	Gen=PWM_GEN_3,OutBits=PWM_OUT_6_BIT;	break;
		case PWM_OUT_7:	Gen=PWM_GEN_3,OutBits=PWM_OUT_7_BIT;	break;
	}
	if(duty==0.0F){
		PWMOutputState(Base, OutBits, false);
	}
	else {
		//配置占空比
		PWMPulseWidthSet(Base, PWMOut, PWMGenPeriodGet(Base, Gen)*duty);
		PWMOutputState(Base, OutBits, true);
	}
	//使能发生器模块
	PWMGenEnable(Base, Gen);
}
