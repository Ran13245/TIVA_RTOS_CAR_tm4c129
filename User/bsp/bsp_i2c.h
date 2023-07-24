/**
 * @file bsp_i2c.h
 * @author cbr (ran13245@outlook.com)
 * @brief 板级I2C驱动
 * @version 0.1
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __BSP_I2C_H_
#define __BSP_I2C_H_

#include <stdint.h>
#include "sensorlib/i2cm_drv.h"

extern tI2CMInstance I2C8MInst;
extern tI2CMWrite8 I2C8MWriteByteInst;
extern volatile bool I2C8MDone;

void init_drv_HardwareI2C(void);
void I2C8MCallback(void *pvData, uint_fast8_t ui8Status);

#endif
