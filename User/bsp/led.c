/**
 * @file led.c
 * @author cbr (ran13245@outlook.com)
 * @brief LED驱动
 * @version 0.1
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "led.h"
#include "bsp_pwm.h"
#include "main.h"
#include "driverlib/gpio.h"
#include "hw_memmap.h"

/*!
 * @brief 设置LED灯，参数应为0或1
 * @param R 
 * @param G 
 * @param B 
 */
void Set_LED(uint8_t R,uint8_t G, uint8_t B){
    if(R>0)GPIOPinWrite(LEDR_GPIO_Port,LEDR_Pin,~LEDR_Pin);
    else GPIOPinWrite(LEDR_GPIO_Port,LEDR_Pin,LEDR_Pin);
    if(G>0)GPIOPinWrite(LEDG_GPIO_Port,LEDG_Pin,~LEDG_Pin);
    else GPIOPinWrite(LEDG_GPIO_Port,LEDG_Pin,LEDG_Pin);
    if(B>0)GPIOPinWrite(LEDB_GPIO_Port,LEDB_Pin,~LEDB_Pin);
    else GPIOPinWrite(LEDB_GPIO_Port,LEDB_Pin,LEDB_Pin);
}
