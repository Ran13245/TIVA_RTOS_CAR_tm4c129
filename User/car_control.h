#ifndef __CAR_CONTOL_H_
#define __CAR_CONTOL_H_

#include <stdint.h>
#include <stdbool.h>
#include "pid.h"

#define BIAS_LINE   10.0F
#define BIAS_ANGLE  3.0F
#define SPIN_INT_RATE   0.5F /*spin剩余量小于该比例后允许被打断*/
#define LINE_INT_RATE   0.5F 

typedef enum __car_mode{
    DISABLE=0,/*禁用,直接控制car_attitude姿态*/
    STOP,/*静止*/
    GO_LINE,/*走直线*/
    TO_POINT,/*去指定点*/
    SPIN/*原地转特定角度*/
}_car_mode;

typedef struct __to_point_parameter
{
    float dir;//旋转方向,以逆时针为1,顺时针为-1
    float R;//运动半径
    float v_bias;//预设速度
    float interrupt_tolerance;//走过该长度之后允许被打断
    bool if_enable_interrupt;//是否已经可以被打断
}_to_point_parameter; 

typedef struct __spin_parameter
{
    float r;//旋转半径
    int16_t circles;//YAW数据溢出次数,正为360->0(逆时针)
    float start_yaw;//旋转开始时的偏航角
    float interrupt_tolerance;//转过该角度之后允许被打断
    bool if_enable_interrupt;//是否已经转过可以被打断的角度
}_spin_parameter;


typedef struct __car_control
{
    _car_mode mode;
    float target_line_distance;
    float current_line_distance;
    pid pid_line_pos;
    float target_spin_angle;//注意旋转角度和YAW不同
    float current_spin_angle;
    pid pid_spin;
    _to_point_parameter to_point_parameter;
    _spin_parameter spin_parameter;
    bool oprate_done;
    bool updated;
}_car_control;

extern _car_control car_control;

void init_Car_Contorl(void);
void Set_Car_Control(float x, float y, float angle);
void Set_Car_V_Bias(float v_bias);
void Car_Control_Update_Input(void);
void Car_Control_Update_Output(void);

#endif
