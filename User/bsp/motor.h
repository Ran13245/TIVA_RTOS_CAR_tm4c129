/**
 * @file motor.h
 * @author cbr (ran13245@outlook.com)
 * @brief 电机驱动
 * @version 0.1
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __MOTOR_H_
#define __MOTOR_H_

#include <stdint.h>
#include <stdbool.h>
#include "pid.h"
#include "config.h"



#define V_ENC_TO_REAL (1.0F/V_REAL_TO_ENC)



//FreeRTOS更新速度任务的   时间间隔(s),时间间隔的倒数(s),频率
#define RTOS_MOTOR_TIME_ITV (TASK_ITV_CAR*0.001F)
#define RTOS_MOTOR_TIME_ITV_REC (1.0F/RTOS_MOTOR_TIME_ITV)




typedef struct __motor
{   
    bool dir;    
    uint32_t PWMBase;
    uint32_t PWMOut;
    uint32_t DirBase1;
    uint8_t  DirPin1;
    uint32_t DirBase2;
    uint8_t  DirPin2;
    pid v_pid;
    int EncSource;//中断读取的编码器变化数
    int total_enc;
    float v_enc;
    float v_real;
    float target_v_enc;
    float duty;
}motor;

typedef enum __motor_choice{
    LEFT=0,
    RIGHT
}_motor_choice;

extern motor motor_LeftFront;
extern motor motor_LeftRear;
extern motor motor_RightFront;
extern motor motor_RightRear;

#if (TEST_BENCH)

extern motor* wheel_left;
extern motor* wheel_right;
/*对特定电机设置*/
void Motor_Set_V_Real(motor* motor,float v_real);
void Motor_Set_V_Enc(motor* motor,float v_enc);
void Motor_Update_Output(motor* motor);
void Motor_Update_Input(motor* motor);
float Motor_Get_V_Real(motor* motor);
void Motor_Clear_Distance(motor* motor);
float Motor_Get_Distance(motor* motor);
/*批量设置*/
void Motor_Set_V_Real_All(float v_real_LF,float v_real_LR,float v_real_RF,float v_real_RR);
void Motor_Set_V_Enc_All(float v_enc_LF,float v_enc_LR,float v_enc_RF,float v_enc_RR);
void Motor_Judge_Accuracy(void);

#endif
/*对外接口*/
void init_motor(void);
void Motor_Update_Input_All(void);
void Motor_Update_Output_All(void);

float Wheel_Get_V_Real(_motor_choice choice);
void Wheel_Set_V_Real(float v_real_left, float v_real_right);
float Wheel_Get_Distance(_motor_choice choice);
void Wheel_Clear_Distance(void);

#endif
