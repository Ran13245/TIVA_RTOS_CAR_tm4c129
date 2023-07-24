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
#include "hw_types.h"
#include "hw_i2c.h"
#include "driverlib/i2c.h"
#include "driverlib/sysctl.h"
#include "driverlib/pin_map.h"
#include "driverlib/gpio.h"
#include <stdbool.h>
#include <stdint.h>
#include "config.h"
#include "sensorlib/i2cm_drv.h"

tI2CMInstance I2C8MInst={0};
tI2CMWrite8 I2C8MWriteByteInst={0};
volatile bool I2C8MDone=0;

extern void delay_ms(uint32_t n);

void I2C8MIntUserHandler(void)
{
//
// Call the I2C master driver interrupt handler.
//
    I2CMIntHandler(&I2C8MInst);
}

void I2C8MCallback(void *pvData, uint_fast8_t ui8Status)
{
    //
    // See if an error occurred.
    //
    if(ui8Status != I2CM_STATUS_SUCCESS)
    {
    //
    // An error occurred, so handle it here if required.
    //
    }
    //
    // Indicate that the I2C transaction has completed.
    //
    I2C8MDone = true;
}


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

    I2CMasterInitExpClk(I2C8_BASE, USER_SYS_FREQ, true);
    I2CIntRegister(I2C8_BASE,I2C8MIntUserHandler);
    I2CMasterIntEnable(I2C8_BASE);
    HWREG(I2C8_BASE + I2C_O_FIFOCTL) = 80008000; // clear I2C FIFOs
    I2CMInit(&I2C8MInst,I2C8_BASE,INT_I2C8,0xff,0xff,USER_SYS_FREQ);
}
