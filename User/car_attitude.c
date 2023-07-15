/**
 * @file car_control.c
 * @author cbr (ran13245@outlook.com)
 * @brief 小车基本姿态代码
 * @version 0.1
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "bsp.h"
#include <math.h>

_car_attitude car_attitude={0};


/*!
 * @brief 初始化小车姿态
 * @param  
 */
void init_Car_Attitude(void){
    Set_PID_Limit(&car_attitude.pid_v_angle,LIMIT_INC_V_ANGLE,LIMIT_POS_V_ANGLE,LIMIT_ITGR_V_ANGLE);
    Set_PID(&car_attitude.pid_v_angle,P_V_ANGLE,I_V_ANGLE,D_V_ANGLE);
}

/*!
 * @brief 更新小车当前姿态
 * @param  
 */
void Car_Attitude_Update_Input(void){
    car_attitude.updated=1;
    float left,right;
    left=Wheel_Get_V_Real(LEFT);
    right=Wheel_Get_V_Real(RIGHT);
    car_attitude.current_v_line=0.5F*(left+right);
    #if V_DEGREE_FROM_IMU
        car_attitude.current_v_angle=imu_data.g_z;//从IMU处获得角速度
    #else
        car_attitude.current_v_angle=0.5F*(right-left)*FRAME_W_HALF_REC*RAD_TO_DEGREE;//用编码器算出小车角速度
    #endif

}

/*!
 * @brief 更新小车姿态输出,增量式PID实现角速度环
 * @param  
 */
void Car_Attitude_Update_Output(void){
    float v_left=0;
    float v_right=0;
    float v_z=0;
    if(car_attitude.flag_stop){
        v_left=0;
        v_right=0;
        v_z=0;
    }
    else {
#if     V_ANGLE_PID
        v_z=PID_Cal_Pos(&car_attitude.pid_v_angle,car_attitude.current_v_angle,car_attitude.target_v_angle);
#else
        v_z=car_attitude.target_v_angle*FRAME_W_HALF*DEGREE_TO_RAD;
#endif
        v_left=car_attitude.target_v_line-v_z;
        v_right=car_attitude.target_v_line+v_z;
    }
    Wheel_Set_V_Real(v_left,v_right);
}


/*!
 * @brief 设置小车姿态
 * @param v_line_target 目标直线速度,mm/s
 * @param v_angle_target 目标角速度,degree/s
 */
void Set_Car_Attitude(float v_line_target,float v_angle_target){
    v_line_target=v_line_target>MAX_V_REAL?MAX_V_REAL:v_line_target;
    v_line_target=v_line_target<-MAX_V_REAL?-MAX_V_REAL:v_line_target;
    v_angle_target=v_angle_target>MAX_V_ANGLE?MAX_V_ANGLE:v_angle_target;
    v_angle_target=v_angle_target<-MAX_V_ANGLE?-MAX_V_ANGLE:v_angle_target;

    car_attitude.target_v_line=v_line_target;
    car_attitude.target_v_angle=v_angle_target;
    if(car_attitude.target_v_line==0 && car_attitude.target_v_angle==0)Set_Car_Stop();
    else Set_Car_Start();
#if (!USE_CAR_CONTROL)
    PID_Clear(&car_attitude.pid_v_angle);
#endif
}


// /*!
//  * @brief 设置小车直线速度和v_z,一般不用
//  * @param v_line_target 目标直线速度 mm/s
//  * @param v_z   旋转补偿速度mm/s
//  */
// void Set_Car_Attitude_With_Vz(float v_line_target,float v_z){
//     car_attitude.target_v_line=v_line_target;
//     car_attitude.target_v_z=v_z;
//     car_attitude.target_v_angle=v_z*FRAME_W_HALF_REC*RAD_TO_DEGREE;
//     if(car_attitude.target_v_line==0 && car_attitude.target_v_z==0)Set_Car_Stop();
//     else Set_Car_Start();
// }

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

void Car_Attitude_Yaw_Update(float v_angle,float time){
    car_attitude.yaw+=v_angle*time;
    
    if(car_attitude.yaw < 0){
        car_attitude.yaw+=360.0F;
        car_control.spin_parameter.circles-=1;
    }
    if(car_attitude.yaw >= 360.0F){
        car_attitude.yaw-=360.0F;
        car_control.spin_parameter.circles+=1;
    }
}
