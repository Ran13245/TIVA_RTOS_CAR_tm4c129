#include "bsp.h"
#include <math.h>

_car_control_vitual car_control_vitual;

static float apply_bias(float data,bool is_x);

void init_Car_Control_Vitual(void){
    Set_PID_Limit(&car_control_vitual.pid_x,LIMIT_INC_VITUAL_X,LIMIT_POS_VITUAL_X,LIMIT_ITGR_VITUAL_X);
    Set_PID_Limit(&car_control_vitual.pid_y,LIMIT_INC_VITUAL_Y,LIMIT_POS_VITUAL_Y,LIMIT_ITGR_VITUAL_Y);
    Set_PID(&car_control_vitual.pid_x,P_VITAUL_X,I_VITAUL_X,D_VITAUL_X);
    Set_PID(&car_control_vitual.pid_y,P_VITAUL_Y,I_VITAUL_Y,D_VITAUL_Y);
}

void Set_Car_Control_Vitual(int x,int y){
    car_control_vitual.updated=1;
    x=x>X_RANGE?X_RANGE:x;
    x=x<-X_RANGE?-X_RANGE:x;
    y=y>Y_RANGE?Y_RANGE:y;
    y=y<-Y_RANGE?-Y_RANGE:y;
    
    car_control_vitual.x=apply_bias(x,1);
    car_control_vitual.y=apply_bias(y,0);
}

void Car_Control_Vitual_Update_Input(void){
    if(!car_control_vitual.updated){
        car_control_vitual.x-=car_control_vitual.x*X_UPDATE_DAMP_RATIO*fabsf(car_attitude.current_v_line)/MAX_V_REAL;
        car_control_vitual.y-=car_control_vitual.y*Y_UPDATE_DAMP_RATIO*fabsf(car_attitude.current_v_angle)/MAX_V_ANGLE;
    }
    if(fabsf(car_control_vitual.x)<X_BIAS){
        car_control_vitual.x=0;
    }
    if(fabsf(car_control_vitual.y)<Y_BIAS){
        car_control_vitual.y=0;
    }
    car_control_vitual.updated=0;
}

void Car_Control_Vitual_Update_Output(void){
#if USE_VITUAL_CONTROL
    float v_line,v_angle;
    v_line=MAX_V_REAL*0.01F*PID_Cal_Pos(&car_control_vitual.pid_x,0,car_control_vitual.x);
    v_angle=MAX_V_ANGLE*0.01F*PID_Cal_Pos(&car_control_vitual.pid_y,0,car_control_vitual.y);
    Set_Car_Attitude(v_line,v_angle);
#endif
}

float apply_bias(float data,bool is_x){
    if(is_x){
        if(data>0){
            return data+X_BIAS;
        }
        else{
            return data-X_BIAS;
        }
    }
    else{
        if(data > 0){
            return data+Y_BIAS;
        }
        else{
            return data-Y_BIAS;
        }
    }
}
