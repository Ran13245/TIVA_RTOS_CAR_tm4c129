#include "config.h"
#include "main.h"
#include "servo.h"
#include "driverlib/pwm.h"
#include "hw_memmap.h"
#include "bsp_pwm.h"

_servo servo_1={0};
_servo servo_2={0};

void init_servo(void){
    servo_1.PWM_BASE=PWM_SERVO_1_BASE;
    servo_1.PWM_OUT=PWM_SERVO_1_OUT;
    servo_1.range_half=SERVO_1_HALF_RANGE;
    servo_2.PWM_BASE=PWM_SERVO_2_BASE;
    servo_2.PWM_OUT=PWM_SERVO_2_OUT;
    servo_2.range_half=SERVO_2_HALF_RANGE;
    Set_Servo_Angle(&servo_1,0);
    Set_Servo_Angle(&servo_1,0);
}

void Set_Servo_Angle(_servo* servo, float angle){    
    if(angle>servo->range_half)angle=servo->range_half;
    if(angle<-servo->range_half)angle=-servo->range_half;
    float duty=SERVO_DUTY_MIN+((angle+servo->range_half)/(2.0F*servo->range_half))*(SERVO_DUTY_MAX-SERVO_DUTY_MIN);
    servo->angle=angle;
    Set_Duty(servo->PWM_BASE,servo->PWM_OUT,duty);
}
