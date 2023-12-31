/**
 * @file pid.c
 * @author cbr (ran13245@outlook.com)
 * @brief PID计算程序
 * @version 0.1
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "pid.h"
#include <stdint.h>


/*!
 * @brief 增量式PID控制
 * @param obj PID参数
 * @param current 当前值,float
 * @param target 目标值,float
 * @return 增量,float
 */
float PID_Cal_Inc(pid *obj, float current, float target){
	float iError;                          //当前误差值
	float Output;                           //控制输出增量值
	
	iError = target - current;            //目标值与当前值之差

	Output = (obj->p * iError)               //E[k]项
			-(obj->i * obj->LastError)       //E[k-1]项
			+(obj->d * obj->PrevError);      //E[k-2]项
    
	obj->PrevError = obj->LastError;         //存储误差，用于下次计算
	obj->LastError = iError;
	
    Output = Output > obj->output_limit_inc ? obj->output_limit_inc :Output;  //控制输出上限幅
    Output = Output < -obj->output_limit_inc ? -obj->output_limit_inc :Output; //控制输出下限幅
	
	return(Output);                         //返回增量值    
}


/*!
 * @brief 位置式PID控制
 * @param obj PID参数
 * @param current 当前值,float
 * @param target 目标值,float
 * @return 受控值,float
 */
float PID_Cal_Pos(pid *obj, float current, float target){
 
    float  iError,     //当前误差
            Output;    //控制输出	
 
    iError = target - current;                   //计算当前误差
	

    obj->IntegralError += obj->i * iError;	            //位置式PID积分项累加
    obj->IntegralError = obj->IntegralError > obj->integral_error_limit ? obj->integral_error_limit:obj->IntegralError;  //积分项上限幅
    obj->IntegralError = obj->IntegralError <-obj->integral_error_limit ? -obj->integral_error_limit:obj->IntegralError; //积分项下限幅
	
    Output = obj->p * iError                        //比例P            
           + obj->d * (iError - obj->LastError)   //微分D
					 + obj->IntegralError;                        //积分I
	
 
    Output = Output > obj->output_limit_pos ? obj->output_limit_pos:Output;  //控制输出上限幅
    Output = Output < -obj->output_limit_pos? -obj->output_limit_pos:Output; //控制输出下限幅
	
	obj->LastError = iError;		  	                     //更新上次误差，用于下次计算 
	return Output;	//返回控制输出值

}

/*!
 * @brief 设置PID参数
 * @param obj PID参数
 * @param p 
 * @param i 
 * @param d 
 */
void Set_PID(pid* obj, float p, float i, float d){
    obj->p=p;
    obj->i=i;
    obj->d=d;
}

/*!
 * @brief 设置PID limit
 * @param obj 
 * @param output_limit_inc 
 * @param output_limit_pos 
 * @param integral_error_limit 
 */
void Set_PID_Limit(pid*obj, float output_limit_inc, float output_limit_pos, float integral_error_limit){
    obj->output_limit_inc=output_limit_inc;
    obj->output_limit_pos=output_limit_pos;
    obj->integral_error_limit=integral_error_limit;
}

/*!
 * @brief 重置PID,用于状态转换时
 * @param obj 
 */
void PID_Clear(pid* obj){
    obj->IntegralError=0;
    obj->LastError=0;
    obj->PrevError=0;
}
