/**
 * @file car_control.c
 * @author cbr (ran13245@outlook.com)
 * @brief 小车控制代码
 * @version 0.0
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "bsp.h"
#include <math.h>

float yaw=0;
_car_attitude car_attitude={0};

static float _abs(float a){
    return a > 0 ? a : -a;
}

/*!
 * @brief 防止漂移:同侧轮中与目标值差值小的视为无打滑
 * @param Front 
 * @param Rear 
 * @return 
 */
static float real_v_anti_drifting(motor* Front,motor* Rear){
    float target=Front->target_v_enc;
    
    if(_abs(Front->v_enc - target) > _abs(Rear->v_enc - target))
        return Rear->v_real;
    else
        return Front->v_real;
}

/*!
 * @brief 初始化小车姿态
 * @param  
 */
void init_Car_Attitude(void){
    Set_PID_Limit(&car_attitude.angle_pid,LIMIT_INC_TURN,LIMIT_POS_TURN,LIMIT_ITGR_TURN);
    Set_PID(&car_attitude.angle_pid,P_TURN,I_TURN,D_TURN);
}

/*!
 * @brief 更新小车当前姿态
 * @param  
 */
void Car_Attitude_Update_Input(void){
car_attitude.updated=1;
#if USE_4_MOTOR
    float left,right;
    left=real_v_anti_drifting(&motor_LeftFront,&motor_LeftRear);
    right=real_v_anti_drifting(&motor_RightFront,&motor_RightRear);
        car_attitude.current_v_line=0.5F*(left+right);
        car_attitude.current_v_z=0.5F*(right-left); 
    #if V_DEGREE_FROM_IMU
        car_attitude.current_v_angle=imu_data.g_z;//从IMU处获得角速度
    #else
        car_attitude.current_v_angle=car_attitude.current_v_z*FRAME_W_HALF_REC*RAD_TO_DEGREE;//用编码器算出小车角速度
    #endif

#else
        car_attitude.current_v_line=0.5F*(motor_LeftFront.v_real+motor_RightFront.v_real);
        car_attitude.current_v_z=0.5F*(motor_RightFront.v_real-motor_LeftFront.v_real);
    #if V_DEGREE_FROM_IMU
        car_attitude.current_v_angle=imu_data.g_z;//从IMU处获得角速度
    #else
        car_attitude.current_v_angle=car_attitude.current_v_z*FRAME_W_HALF_REC*RAD_TO_DEG;//用编码器算出小车角速度
    #endif

#endif
}

/*!
 * @brief 更新小车姿态输出,增量式PID实现角速度环
 * @param  
 */
void Car_Attitude_Update_Output(void){
    static float v_left=0;
    static float v_right=0;
    if(car_attitude.flag_stop){
        car_attitude.target_v_z=0;
        v_left=0;
        v_right=0;
    }
    else {
        v_left=car_attitude.target_v_line-car_attitude.target_v_z;
        v_right=car_attitude.target_v_line+car_attitude.target_v_z;
    }
    Motor_Set_V_Real_All(v_left,v_left,v_right,v_right);
}


/*!
 * @brief 设置小车姿态
 * @param v_line_target 目标直线速度,mm/s
 * @param v_angle_target 目标角速度,degree/s
 */
void Set_Car_Attitude(float v_line_target,float v_angle_target){
    car_attitude.target_v_line=v_line_target;
    car_attitude.target_v_angle=v_angle_target;
    car_attitude.target_v_z=v_angle_target*FRAME_W_HALF*DEGREE_TO_RAD;
    if(car_attitude.target_v_line==0 && car_attitude.target_v_angle==0)Set_Car_Stop();
    else Set_Car_Start();
}


/*!
 * @brief 设置小车姿态 包括v_z
 * @param v_line_target 目标直线速度 mm/s
 * @param v_z   旋转补偿速度mm/s
 */
void Set_Car_Attitude_With_Vz(float v_line_target,float v_z){
    car_attitude.target_v_line=v_line_target;
    car_attitude.target_v_z=v_z;
    car_attitude.target_v_angle=v_z*FRAME_W_HALF_REC*RAD_TO_DEGREE;
    if(car_attitude.target_v_line==0 && car_attitude.target_v_z==0)Set_Car_Stop();
    else Set_Car_Start();
}

/*!
 * @brief 设置小车停止
 * @param  
 */
void Set_Car_Stop(void){
    car_attitude.flag_stop=1;
}

/*!
 * @brief 设置小车启动
 * @param  
 */
void Set_Car_Start(void){
    car_attitude.flag_stop=0;
}

// void Set_Car_Rotate(float angle){
//     static float current_yaw;
//     current_yaw = car_attitude.yaw;
// }

