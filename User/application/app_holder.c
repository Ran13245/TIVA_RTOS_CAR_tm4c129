#include "bsp.h"
#include "application.h"
#include "app_holder.h"
#include <math.h>

_holder holder_2d={0};

static void pSet_App_Holder_AngleInSpace(_holder* holder, float angle_in_space);
static void pSet_App_Holder_AnglePitch(_holder* holder, float angle_pitch);
static void pApp_Update_Holder(_holder* holder);

void init_app_holder(void){
    holder_2d.pServo_lower=&servo_1;
    holder_2d.pServo_upper=&servo_2;
    pSet_App_Holder_AngleInSpace(&holder_2d,0);
}

void pSet_App_Holder_AngleInSpace(_holder* holder, float angle_in_space){
    if(angle_in_space<0)angle_in_space+=360.0f;
    if(angle_in_space>=360.0f)angle_in_space-=360.0f;
    holder->angle_in_space=angle_in_space;
}

void pSet_App_Holder_AnglePitch(_holder* holder, float angle_pitch){
    if(angle_pitch>HOLDER_PITCH_RANGE)angle_pitch=HOLDER_PITCH_RANGE;
    if(angle_pitch<0.0F)angle_pitch=0.0F;
    holder->angle_pitch=angle_pitch;
    Set_Servo_Angle(holder->pServo_upper,angle_pitch);
}

void pApp_Update_Holder(_holder* holder){
    holder->angle_in_car=fmodf(holder->angle_in_space+360-car_attitude.yaw,360);
    holder->angle_in_servo=fmodf(holder->angle_in_car+360-SERVO_CRITICAL,360)-180;
    Set_Servo_Angle(holder->pServo_lower,holder->angle_in_servo);
}

void Set_App_Holder2d_AngleInSpace(float angle_in_space){
    pSet_App_Holder_AngleInSpace(&holder_2d,angle_in_space);
}

void Set_App_Holder2d_AnglePitch(float angle_pitch){
    pSet_App_Holder_AnglePitch(&holder_2d,angle_pitch);
}

void App_Update_Holder_All(void){
    pApp_Update_Holder(&holder_2d);
}
