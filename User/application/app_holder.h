#ifndef __APP_HOLDER_2D_H_
#define __APP_HOLDER_2D_H_

#include <stdint.h>
#include <stdbool.h>

#include "servo.h"

typedef struct __holder
{
    _servo* pServo_upper;
    _servo* pServo_lower;
    float angle_in_space;/*0~360,舵机在导航坐标系的角度位置*/
    float angle_in_car;/*0~360,舵机经过航偏角补偿之后 在小车坐标系的位置*/
    float angle_in_servo;/*-180~180,舵机在舵机坐标系中的位置*/
    float angle_pitch;/*俯仰角,0~HOLDER_PITCH_RANGE*/
}_holder;

extern _holder holder_2d;

void init_app_holder(void);

void Set_App_Holder2d_AngleInSpace(float angle_in_space);
void Set_App_Holder2d_AnglePitch(float angle_pitch);
void App_Update_Holder_All(void);

#endif
