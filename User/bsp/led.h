/**
 * @file led.h
 * @author cbr (ran13245@outlook.com)
 * @brief LED驱动
 * @version 0.1
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __LED_H_
#define __LED_H_

#include "bsp_pwm.h"
#include <stdint.h>


void Set_LED(uint8_t R,uint8_t G, uint8_t B);

#endif
