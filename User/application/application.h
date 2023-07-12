#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#define SERVO_L_BASE           GPIO_PORTP_BASE          
#define SERVO_R_BASE           GPIO_PORTP_BASE
#define SERVO_L_PIN            GPIO_PIN_2
#define SERVO_R_PIN            GPIO_PIN_3

/*像素转化为角度*/
#define PIXEL_TO_ANGLE        -0.01F

#include "app_servo.h"

void init_app(void);

#endif
