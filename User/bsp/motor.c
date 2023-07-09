/**
 * @file motor.c
 * @author cbr (ran13245@outlook.com)
 * @brief 电机驱动
 * @version 0.1
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "main.h"
#include "motor.h"
#include <stdint.h>
#include "pid.h"
#include "bsp_pwm.h"
#include "driverlib/gpio.h"
#include "hw_memmap.h"
#include "driverlib/pwm.h"
#include "car_control.h"
#include "config.h"

motor motor_LeftFront={0};
motor motor_LeftRear={0};
motor motor_RightFront={0};
motor motor_RightRear={0};

/*!
 * @brief 绝对值,输出限幅
 * @param num float
 * @return float
 */
static float abs(float num){
    float rtn;
    rtn= num > 0 ? num : -num;
    return rtn;
}

static float duty_limit(float duty){
    float rtn;
    rtn= rtn > MAX_MOTOR_DUTY ? MAX_MOTOR_DUTY : rtn;
    rtn= rtn <-MAX_MOTOR_DUTY ?-MAX_MOTOR_DUTY : rtn;
    return rtn;
}

/*!
 * @brief 初始化电机
 * @param  void
 */
void init_motor(void){
    motor_LeftFront.dir=LF_DIR;
    motor_LeftRear.dir=LR_DIR;
    motor_RightFront.dir=RF_DIR;
    motor_RightRear.dir=RR_DIR;

    Set_PID_Limit(&motor_LeftFront.v_pid,LIMIT_INC_LF,LIMIT_POS_LF,LIMIT_ITGR_LF);
    Set_PID_Limit(&motor_LeftRear.v_pid,LIMIT_INC_LR,LIMIT_POS_LR,LIMIT_ITGR_LR);
    Set_PID_Limit(&motor_RightFront.v_pid,LIMIT_INC_RF,LIMIT_POS_RF,LIMIT_ITGR_RF);
    Set_PID_Limit(&motor_RightRear.v_pid,LIMIT_INC_RR,LIMIT_POS_RR,LIMIT_ITGR_RR);

    Set_PID(&motor_LeftFront.v_pid,P_LF,I_LF,D_LF);
    Set_PID(&motor_LeftRear.v_pid,P_LR,I_LR,D_LR);
    Set_PID(&motor_RightFront.v_pid,P_RF,I_RF,D_RF);
    Set_PID(&motor_RightRear.v_pid,P_RR,I_RR,D_RR);

    motor_LeftFront.DirBase1=LFMotor_RotateDirectionCtrl1_GPIO_Port;
    motor_LeftFront.DirPin1=LFMotor_RotateDirectionCtrl1_Pin;
    motor_LeftFront.DirBase2=LFMotor_RotateDirectionCtrl2_GPIO_Port;
    motor_LeftFront.DirPin2=LFMotor_RotateDirectionCtrl2_Pin;
    motor_LeftFront.PWMBase=PWM_LF_BASE;
    motor_LeftFront.PWMOut=PWM_LF_OUT;

    motor_LeftRear.DirBase1=LRMotor_RotateDirectionCtrl1_GPIO_Port;
    motor_LeftRear.DirPin1=LRMotor_RotateDirectionCtrl1_Pin;
    motor_LeftRear.DirBase2=LRMotor_RotateDirectionCtrl2_GPIO_Port;
    motor_LeftRear.DirPin2=LRMotor_RotateDirectionCtrl2_Pin;
    motor_LeftRear.PWMBase=PWM_LR_BASE;
    motor_LeftRear.PWMOut=PWM_LR_OUT;

    motor_RightFront.DirBase1=RFMotor_RotateDirectionCtrl1_GPIO_Port;
    motor_RightFront.DirPin1=RFMotor_RotateDirectionCtrl1_Pin;
    motor_RightFront.DirBase2=RFMotor_RotateDirectionCtrl2_GPIO_Port;
    motor_RightFront.DirPin2=RFMotor_RotateDirectionCtrl2_Pin;
    motor_RightFront.PWMBase=PWM_RF_BASE;
    motor_RightFront.PWMOut=PWM_RF_OUT;

    motor_RightRear.DirBase1=RRMotor_RotateDirectionCtrl1_GPIO_Port;
    motor_RightRear.DirPin1=RRMotor_RotateDirectionCtrl1_Pin;
    motor_RightRear.DirBase2=RRMotor_RotateDirectionCtrl2_GPIO_Port;
    motor_RightRear.DirPin2=RRMotor_RotateDirectionCtrl2_Pin;
    motor_RightRear.PWMBase=PWM_RR_BASE;
    motor_RightRear.PWMOut=PWM_RR_OUT;

}

/*!
 * @brief 设置电机目标真实速度,单位mm/s
 * @param motor 电机obj
 * @param v_real 目标速度mm/s,float
 */
void Motor_Set_V_Real(motor* motor,float v_real){
    int v_enc=V_REAL_TO_ENC*v_real;
    Motor_Set_V_Enc(motor,v_enc);
}

/*!
 * @brief 获取电机当前真实速度,单位mm/s
 * @param motor 电机obj
 * @return 当前速度mm/s,float
 */
float Motor_Get_V_Real(motor* motor){
    return motor->v_real;
}

/*!
 * @brief 设置电机目标编码器速度,单位 编码器单位/s
 * @param motor 电机obj
 * @param v_enc 编码器速度,float
 */
void Motor_Set_V_Enc(motor* motor,float v_enc){
    v_enc = v_enc > MAX_V_ENC ? MAX_V_ENC : v_enc;
    v_enc = v_enc < -MAX_V_ENC? -MAX_V_ENC: v_enc;
    motor->target_v_enc=v_enc;
}

/*!
 * @brief 更新电机输出,调用增量式pid
 * @param motor 电机obj
 */
void Motor_Update_Output(motor* motor){
    if(car_attitude.flag_stop){
        motor->duty=0;
        GPIOPinWrite(motor->DirBase1,motor->DirPin1,~(motor->DirPin1));
        GPIOPinWrite(motor->DirBase2,motor->DirPin2,~(motor->DirPin2));
    }
    else{
        motor->duty+=ZOOM_PID_TO_DUTY*PID_Cal_Inc(&(motor->v_pid),motor->v_enc,motor->target_v_enc);
        motor->duty=duty_limit(motor->duty);
            if(motor->duty > 0){
                GPIOPinWrite(motor->DirBase1,motor->DirPin1,motor->DirPin1);
                GPIOPinWrite(motor->DirBase2,motor->DirPin2,~(motor->DirPin2));
            }
            else {
                GPIOPinWrite(motor->DirBase1,motor->DirPin1,~(motor->DirPin1));
                GPIOPinWrite(motor->DirBase2,motor->DirPin2,motor->DirPin2);
            }
    }
    Set_Duty(motor->PWMBase,motor->PWMOut,abs(motor->duty));
}

/*!
 * @brief 更新电机当前速度
 * @param motor 电机obj
 */
void Motor_Update_Input(motor* motor){
    motor->v_enc=motor->EncSource*RTOS_MOTOR_TIME_ITV_REC;
    motor->total_enc+=motor->EncSource;
    if(!motor->dir)motor->v_enc = -motor->v_enc;
    motor->v_real=motor->v_enc*V_ENC_TO_REAL;
    motor->EncSource=0;
}

/*!
 * @brief 更新所有电机速度
 * @param  
 */
void Motor_Update_Input_All(void){
#if     USE_4_MOTOR    
    Motor_Update_Input(&motor_LeftRear);
    Motor_Update_Input(&motor_RightRear);
#endif
    Motor_Update_Input(&motor_LeftFront);
    Motor_Update_Input(&motor_RightFront);
}

/*!
 * @brief 更新所有电机输出
 * @param  
 */
void Motor_Update_Output_All(void){
#if     USE_4_MOTOR
    Motor_Update_Output(&motor_LeftRear);
    Motor_Update_Output(&motor_RightRear);
#endif
    Motor_Update_Output(&motor_LeftFront);
    Motor_Update_Output(&motor_RightFront);
}

/*!
 * @brief 批量设置电机真实速度,单位mm/s
 * @param v_real_LF 
 * @param v_real_LR 
 * @param v_real_RF 
 * @param v_real_RR 
 */
void Motor_Set_V_Real_All(float v_real_LF,float v_real_LR,float v_real_RF,float v_real_RR){
#if     USE_4_MOTOR
    Motor_Set_V_Real(&motor_LeftRear,v_real_LR);
    Motor_Set_V_Real(&motor_RightRear,v_real_RR);
#endif
    Motor_Set_V_Real(&motor_LeftFront,v_real_LF);
    Motor_Set_V_Real(&motor_RightFront,v_real_RF);
}

/*!
 * @brief 批量设置电机编码器速度
 * @param v_enc_LF 
 * @param v_enc_LR 
 * @param v_enc_RF 
 * @param v_enc_RR 
 */
void Motor_Set_V_Enc_All(float v_enc_LF,float v_enc_LR,float v_enc_RF,float v_enc_RR){
#if     USE_4_MOTOR
    Motor_Set_V_Enc(&motor_LeftRear,v_enc_LR);
    Motor_Set_V_Enc(&motor_RightRear,v_enc_RR);
#endif
    Motor_Set_V_Enc(&motor_LeftFront,v_enc_LF);
    Motor_Set_V_Enc(&motor_RightFront,v_enc_RF);
}
