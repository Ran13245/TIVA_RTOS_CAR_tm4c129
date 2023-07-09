/**
 * @file bsp_i2c.c
 * @author cbr (ran13245@outlook.com)
 * @brief 板级I2C驱动
 * @version 0.1
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "main.h"
#include "bsp_i2c.h"
#include "tm4c1290ncpdt.h"
#include "hw_memmap.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include <stdbool.h>
#include <stdint.h>

extern void delay_ms(uint32_t n);
/*!
 * @brief 初始化I2C
 * @note 只支持I2C0_BASE
 */
void init_drv_HardwareI2C()
{	


    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C8);
    GPIOPinConfigure(GPIO_PA2_I2C8SCL);
    GPIOPinConfigure(GPIO_PA3_I2C8SDA);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_3);
    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_2);

    I2CMasterInitExpClk(I2C8_BASE, SysCtlClockGet(), true);
}
