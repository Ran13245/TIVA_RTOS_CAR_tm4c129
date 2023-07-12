#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#define SERVO_L_BASE           GPIO_PORTP_BASE          
#define SERVO_R_BASE           GPIO_PORTP_BASE
#define SERVO_L_PIN            GPIO_PIN_2
#define SERVO_R_PIN            GPIO_PIN_3

/*角速度转化为舵机纠正值*/
#define V_ANGLE_TO_SERVO        0.1F

#include "app_servo.h"

void init_app(void);

#endif
