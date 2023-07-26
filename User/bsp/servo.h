#ifndef __SERVO_H_
#define __SERVO_H_

#include <stdint.h>
#include <stdbool.h>

//0.5ms~2.5ms
#define SERVO_DUTY_MIN 0.19
#define SERVO_DUTY_MAX 0.95

typedef struct __servo
{
    uint32_t PWM_BASE;
    uint32_t PWM_OUT;
    float range_half;
    float angle;
}_servo;

extern _servo servo_1;
extern _servo servo_2;

void init_servo(void);
void Set_Servo_Angle(_servo* servo, float angle);

#endif 
