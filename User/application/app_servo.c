#include "bsp.h"
#include "application.h"
#include <math.h>
servo servo_1;//正向
servo servo_2;//反向

float angle_in_space;/*0~360,舵机在导航坐标系的角度位置*/
float angle_in_car;/*0~360,舵机经过航偏角补偿之后 在小车坐标系的位置*/
float angle_in_servo;/*-180~180,舵机在舵机坐标系中的位置*/

void init_app_servo(void){
    servo_1.dev=Soft_PWM_Add(SERVO_L_BASE,SERVO_L_PIN,SERVO_PWM_FULL_WIDTH_US);
    servo_2.dev=Soft_PWM_Add(SERVO_R_BASE,SERVO_R_PIN,SERVO_PWM_FULL_WIDTH_US);
    Set_target_servo(0);
}

/*!
 * @brief -SERVO_HALF_RANGE ~ +SERVO_HALF_RANGE,逆时针为正
 * @param servo 
 * @param degree 
 */
void Servo_Set_Degree(servo* servo,float degree){
    if(degree>SERVO_HALF_RANGE)degree=SERVO_HALF_RANGE;
    if(degree<-SERVO_HALF_RANGE)degree=-SERVO_HALF_RANGE;
    float duty=SERVO_DUTY_MIN+((degree+SERVO_HALF_RANGE)/(2.0F*SERVO_HALF_RANGE))*(SERVO_DUTY_MAX-SERVO_DUTY_MIN);
    Set_Duty_Soft(servo->dev,duty);
}


void Set_target_servo(float set_angle_in_space){
    angle_in_space=set_angle_in_space;
}

void Set_target_servo_flag(uint8_t flag){
    if(flag==0){
        angle_in_space+=0;
    }
    if(flag==1){
        angle_in_space-=5;
    }
    if(flag==2){
        angle_in_space+=5;
    }
}

void App_Update_Servo(void){
    angle_in_car=fmodf(angle_in_space+360-car_attitude.yaw,360);
    angle_in_servo=fmodf(angle_in_car+360-SERVO_CRITICAL,360)-180;
    Servo_Set_Degree(&servo_1,angle_in_servo*0.5F);
    Servo_Set_Degree(&servo_2,angle_in_servo*0.5F);
}


