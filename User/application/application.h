#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#define SERVO_LR_BASE           GPIO_PORTP_BASE          
#define SERVO_UD_BASE           GPIO_PORTP_BASE
#define SERVO_LR_PIN            GPIO_PIN_2
#define SERVO_UD_PIN            GPIO_PIN_3

#include "app_servo.h"

void init_app(void);

#endif
