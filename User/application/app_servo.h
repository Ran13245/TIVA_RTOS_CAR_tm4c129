#ifndef __APP_SERVO_H_
#define __APP_SERVO_H_

#include <stdint.h>

#include "software_pwm.h"
//舵机角度绝对值范围的一半,degree
#define SERVO_HALF_RANGE 90

//20ms
#define SERVO_PWM_FULL_WIDTH_US 20000

//1.0ms~2.0ms
#define SERVO_DUTY_MIN 0.05
#define SERVO_DUTY_MAX 0.1


typedef struct 
{
    _soft_pwm_device* dev;
    float degree;//有符号,0表示正中
}servo;

//左右方向的舵机接左后接口;上下方向的舵机接右后接口
extern servo servo_LeftRight;
extern servo servo_UpDown;

void init_app_servo(void);
void Servo_Set_Degree(servo* servo,float degree);
void Servo_Set_Degree_All(float LRdegree, float UDdegree);

#endif

