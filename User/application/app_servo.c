#include "bsp.h"
#include "application.h"
servo servo_LeftRight;
servo servo_UpDown;

void init_app_servo(void){
    servo_LeftRight.dev=Soft_PWM_Add(SERVO_LR_BASE,SERVO_LR_PIN,SERVO_PWM_FULL_WIDTH_US);
    servo_UpDown.dev=Soft_PWM_Add(SERVO_UD_BASE,SERVO_UD_PIN,SERVO_PWM_FULL_WIDTH_US);
    Servo_Set_Degree_All(0,0);
}

void Servo_Set_Degree(servo* servo,float degree){
    if(degree>SERVO_HALF_RANGE)degree=SERVO_HALF_RANGE;
    if(degree<-SERVO_HALF_RANGE)degree=-SERVO_HALF_RANGE;
    servo->degree=degree;
    float duty=SERVO_DUTY_MIN+((degree+SERVO_HALF_RANGE)/(2.0F*SERVO_HALF_RANGE))*(SERVO_DUTY_MAX-SERVO_DUTY_MIN);
    Set_Duty_Soft(servo->dev,duty);
}

void Servo_Set_Degree_All(float LRdegree, float UDdegree){
    Servo_Set_Degree(&servo_LeftRight,LRdegree);
    Servo_Set_Degree(&servo_UpDown,UDdegree);
}


