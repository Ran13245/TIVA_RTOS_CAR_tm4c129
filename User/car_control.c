#include "bsp.h"
#include <math.h>

_car_control car_control;

static void get_current_distance(void);
static void get_current_spin_angle(void);
static float add_bias(float target,bool if_spin);
/*!
 * @brief 初始化小车控制
 * @param  
 */
void init_Car_Contorl(void){
    Set_PID_Limit(&car_control.pid_line_pos,LIMIT_INC_POS,LIMIT_POS_POS,LIMIT_ITGR_POS);
    Set_PID_Limit(&car_control.pid_spin,LIMIT_INC_SPIN,LIMIT_POS_SPIN,LIMIT_ITGR_SPIN);
    Set_PID(&car_control.pid_line_pos,P_POS,I_POS,D_POS);
    Set_PID(&car_control.pid_spin,P_SPIN,I_SPIN,D_SPIN);
    car_control.spin_parameter.if_enable_interrupt=1;
}

/*!
 * @brief 控制小车
 *  自动选择模式,
 *      全为0停止
 *      只有x不为0走直线
 *      只有x,y不为0去指定点
 *      只有y,angle原地旋转,y表示旋转半径,不分正负,可为0
 *      全不为0非法,停止
 *  以前轮中点为原点,中轴向前为x正向,左侧为y正向
 * @param x 目标点x值 mm
 * @param y 目标点y值 mm
 * @param angle degree
 */
void Set_Car_Control(float x, float y, float angle){
#if ANTI_SPIN_INT
    if(car_control.spin_parameter.if_enable_interrupt){
#endif
        if(y==0 && x!=0 && angle==0){
            car_control.mode=GO_LINE;
            car_control.target_line_distance=x;
            Wheel_Clear_Distance();
            PID_Clear(&car_control.pid_line_pos);
            PID_Clear(&car_attitude.pid_v_angle);
            car_control.spin_parameter.if_enable_interrupt=1;
            car_control.target_line_distance=add_bias(car_control.target_line_distance,0);
        }
        else if(y!=0 && x!=0 && angle==0){
            car_control.mode=TO_POINT;
            car_control.to_point_parameter.dir=y>0?1.0F:-1.0F;
            y=fabsf(y);
            car_control.to_point_parameter.R=(x*x+y*y)/(2*y);
            car_control.target_line_distance=car_control.to_point_parameter.R * asin(x/car_control.to_point_parameter.R);
            Wheel_Clear_Distance();
            PID_Clear(&car_control.pid_line_pos);
            PID_Clear(&car_attitude.pid_v_angle);
            car_control.spin_parameter.if_enable_interrupt=1;
            car_control.target_line_distance=add_bias(car_control.target_line_distance,0);
        }
        else if(x==0 && angle!=0){
            car_control.mode=SPIN;
            car_control.target_spin_angle=angle;
            car_control.spin_parameter.interrupt_tolerance=fabsf(angle)*SPIN_INT_RATE;
            car_control.spin_parameter.start_yaw=car_attitude.yaw;
            car_control.spin_parameter.r=fabsf(y);
            car_control.spin_parameter.circles=0;
            car_control.spin_parameter.if_enable_interrupt=0;
            PID_Clear(&car_control.pid_spin);
            PID_Clear(&car_attitude.pid_v_angle);
            car_control.target_spin_angle=add_bias(car_control.target_spin_angle,1);
            car_control.spin_parameter.interrupt_tolerance=add_bias(car_control.spin_parameter.interrupt_tolerance,1);
        }
        else{
            car_control.mode=STOP;
        }
#if ANTI_SPIN_INT
    }
#endif
}

/*!
 * @brief 更新小车控制当前状态
 * @param  
 */
void Car_Control_Update_Input(void){
    switch (car_control.mode)
    {
        case GO_LINE:{
            get_current_distance();
            if(fabsf(car_control.current_line_distance - car_control.target_line_distance) <  BIAS_LINE) {
                Set_Car_Control(0,0,0);
            }
            break;
        }
            
        case TO_POINT:{
            get_current_distance();
            if(fabsf(car_control.current_line_distance - car_control.target_line_distance) < BIAS_LINE) {
                Set_Car_Control(0,0,0);
            }
            break;
        }
            
        case SPIN:{
            get_current_spin_angle();
            if(fabsf(car_control.current_spin_angle - car_control.target_spin_angle) < BIAS_ANGLE) {
                car_control.spin_parameter.if_enable_interrupt=1;
                Set_Car_Control(0,0,0);
            }
            if(fabsf(car_control.current_spin_angle - car_control.target_spin_angle) <  car_control.spin_parameter.interrupt_tolerance){
                car_control.spin_parameter.if_enable_interrupt=1;
            }
            break;
        }
            
        default:{
            break;
        }
    }
}

/*!
 * @brief 更新小车控制输出
 * @param  
 */
void Car_Control_Update_Output(void){
#if     USE_CAR_CONTROL
    float target_v_line,target_v_angle;
    switch (car_control.mode)
    {
        case GO_LINE:{
            target_v_line=PID_Cal_Pos(&car_control.pid_line_pos,car_control.current_line_distance, car_control.target_line_distance)+ car_control.to_point_parameter.v_bias;
            Set_Car_Attitude(target_v_line,0);
            break;
        }
            
        case TO_POINT:{
            target_v_line=PID_Cal_Pos(&car_control.pid_line_pos,car_control.current_line_distance,car_control.target_line_distance)+ car_control.to_point_parameter.v_bias;
            target_v_angle=car_control.to_point_parameter.dir * target_v_line / car_control.to_point_parameter.R * RAD_TO_DEGREE;
            Set_Car_Attitude(target_v_line,target_v_angle);
            break;
        }
            
        case SPIN:{
            target_v_angle=PID_Cal_Pos(&car_control.pid_spin,car_control.current_spin_angle,car_control.target_spin_angle);
            target_v_line=fabsf(target_v_angle)*DEGREE_TO_RAD*car_control.spin_parameter.r;
            Set_Car_Attitude(target_v_line,target_v_angle);
            break;
        }
            
        default:{
            Set_Car_Attitude(0,0);
            break;
        }
    }
#endif
}

void get_current_distance(void){
    car_control.current_line_distance = 0.5F*(Wheel_Get_Distance(LEFT)+Wheel_Get_Distance(RIGHT));
}

void get_current_spin_angle(void){
    car_control.current_spin_angle = car_control.spin_parameter.circles*360.0F + car_attitude.yaw - car_control.spin_parameter.start_yaw;
}

float add_bias(float target,bool if_spin){
    bool signal = target < 0;
    if(if_spin){
        if(signal){
            return target - BIAS_ANGLE;
        }
        else{
            return target + BIAS_ANGLE;
        }
    }
    else{
        if(signal){
            return target - BIAS_LINE;
        }
        else{
            return target + BIAS_LINE;
        }
    }
    
}

void Set_Car_V_Bias(float v_bias){
    car_control.to_point_parameter.v_bias=v_bias;
}

