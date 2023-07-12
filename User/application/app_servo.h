#ifndef __APP_SERVO_H_
#define __APP_SERVO_H_

#include <stdint.h>

#include "software_pwm.h"
//舵机角度绝对值范围的一半,degree
#define SERVO_HALF_RANGE 90
/*舵机临界角,到达该角度时停车翻转舵机,该值是在小车坐标系中,在舵机坐标系中为-180*/
#define SERVO_CRITICAL 270
//20ms
#define SERVO_PWM_FULL_WIDTH_US 20000

//0.5ms~2.5ms
#define SERVO_DUTY_MIN 0.025
#define SERVO_DUTY_MAX 0.125


typedef struct 
{
    _soft_pwm_device* dev;

}servo;

//左右方向的舵机接左后接口;上下方向的舵机接右后接口
extern servo servo_right;
extern servo servo_left;

void init_app_servo(void);
void Servo_Set_Degree(servo* servo,float degree);

void Set_target_servo_flag(uint8_t flag,short data);
void Set_target_servo(float set_angle_in_space);
void App_Update_Servo(void);


#endif

