#include "bsp.h"


#ifdef WAVE_UART
//波形显示缓冲区
unsigned char DataScope_OutPut_Buffer[42] = {0};	

static void Float2Byte(float *target,unsigned char *buf,unsigned char beg);
static void DataScope_Get_Channel_Data(float Data,unsigned char Channel);
static unsigned char DataScope_Data_Generate(unsigned char Channel_Number);


/*
 * 函数功能：将待发送通道的单精度浮点数据写入发送缓冲区
 * Data：通道数据
 * Channel：选择通道（1-10）
 * beg:指定从数组第几个元素开始写入
 */
void DataScope_Get_Channel_Data(float Data,unsigned char Channel)
{
	if((Channel > 10) || (Channel == 0)) 
		return;  
  else
  {
     switch (Channel)
		{
      case 1:  Float2Byte(&Data,DataScope_OutPut_Buffer,1);  break;
      case 2:  Float2Byte(&Data,DataScope_OutPut_Buffer,5);  break;
		  case 3:  Float2Byte(&Data,DataScope_OutPut_Buffer,9);  break;
		  case 4:  Float2Byte(&Data,DataScope_OutPut_Buffer,13); break;
		  case 5:  Float2Byte(&Data,DataScope_OutPut_Buffer,17); break;
		  case 6:  Float2Byte(&Data,DataScope_OutPut_Buffer,21); break;
		  case 7:  Float2Byte(&Data,DataScope_OutPut_Buffer,25); break;
		  case 8:  Float2Byte(&Data,DataScope_OutPut_Buffer,29); break;
		  case 9:  Float2Byte(&Data,DataScope_OutPut_Buffer,33); break;
		  case 10: Float2Byte(&Data,DataScope_OutPut_Buffer,37); break;
		}
  }	 
}

/*
 * 函数功能：生成 DataScopeV1.0 能正确识别的帧格式
 * Channel_Number：需要发送的通道个数
 * 返回发送缓冲区数据个数
 */
unsigned char DataScope_Data_Generate(unsigned char Channel_Number)
{
	if ( (Channel_Number > 10) || (Channel_Number == 0) ) 
	{ 
		return 0; 
	}
  else
  {	
	 DataScope_OutPut_Buffer[0] = '$';  //帧头
		
	 switch(Channel_Number)   
   { 
		 case 1:   DataScope_OutPut_Buffer[5]  =  5; return  6;  
		 case 2:   DataScope_OutPut_Buffer[9]  =  9; return 10;
		 case 3:   DataScope_OutPut_Buffer[13] = 13; return 14; 
		 case 4:   DataScope_OutPut_Buffer[17] = 17; return 18;
		 case 5:   DataScope_OutPut_Buffer[21] = 21; return 22;  
		 case 6:   DataScope_OutPut_Buffer[25] = 25; return 26;
		 case 7:   DataScope_OutPut_Buffer[29] = 29; return 30; 
		 case 8:   DataScope_OutPut_Buffer[33] = 33; return 34; 
		 case 9:   DataScope_OutPut_Buffer[37] = 37; return 38;
     case 10:  DataScope_OutPut_Buffer[41] = 41; return 42; 
   }	 
  }
	return 0;
}


  

/*
 * 函数功能：将单精度浮点数据转成4字节数据并存入指定地址 
 * target:目标单精度数据
 * buf:待写入数组
 * beg:指定从数组第几个元素开始写入
 */
void Float2Byte(float *target,unsigned char *buf,unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char*)target;	  
    buf[beg]   = point[0];
    buf[beg+1] = point[1];
    buf[beg+2] = point[2];
    buf[beg+3] = point[3];
}
 




//待输出数据数量
int cnt = 0;


/*!
 * @brief 载入需要输出的数据,输出频道按载入顺序递增
 * @param data 载入数据,float
 */
void DataScope_Load(float data){
	cnt++;
    DataScope_Get_Channel_Data(data,cnt);
    
}


/*!
 * @brief 输出波形到上位机
 * @param  
 */
void DataScope(void)
{   
	//计数需要的变量
	int Send_Count,i;

	//打印几个数据就在这里改为几
	Send_Count = DataScope_Data_Generate(cnt);
	for( i = 0 ; i < Send_Count; i++) 
	{	

		printf_user(WAVE_UART,"%c", DataScope_OutPut_Buffer[i]); 

	}
	

	cnt=0;
}
#endif


#ifdef Jetson_UART

uint8_t mcu_to_jts_trans[MCU_TO_JTS_SIZE];
_mcu_to_jts mcu_to_jts;
_jts_to_mcu jts_to_mcu;

/*!
 * @brief 数据同步到Jetson
 * @param  
 */
void Upload_To_JTS(void){
	mcu_to_jts.acc_x=(int32_t)(imu_data.a_x*1000);
	mcu_to_jts.acc_y=(int32_t)(imu_data.a_y*1000);
	mcu_to_jts.acc_z=(int32_t)(imu_data.a_z*1000);
	
	mcu_to_jts.flag=car_attitude.updated;
	mcu_to_jts.timestamp = imu_timestamp;
	
	mcu_to_jts.gyr_x=(int32_t)(imu_data.g_x*1000);
	mcu_to_jts.gyr_y=(int32_t)(imu_data.g_y*1000);
	mcu_to_jts.gyr_z=(int32_t)(imu_data.g_z*1000);
	mcu_to_jts.voltage=(short)(main_voltage*1000);
	mcu_to_jts.x_speed=car_attitude.current_v_line;
	mcu_to_jts.y_speed=0;
	mcu_to_jts.z_speed=car_attitude.current_v_angle*1000;

	mcu_to_jts_trans[0]=UART_HEAD;
	mcu_to_jts_trans[1]=mcu_to_jts.flag;
	mcu_to_jts_trans[2]=mcu_to_jts.timestamp>>24;
	mcu_to_jts_trans[3]=mcu_to_jts.timestamp>>16;
	mcu_to_jts_trans[4]=mcu_to_jts.timestamp>>8;
	mcu_to_jts_trans[5]=mcu_to_jts.timestamp;
	mcu_to_jts_trans[6]=mcu_to_jts.x_speed>>8;
	mcu_to_jts_trans[7]=mcu_to_jts.x_speed;
	mcu_to_jts_trans[8]=mcu_to_jts.y_speed>>8;
	mcu_to_jts_trans[9]=mcu_to_jts.y_speed;
	mcu_to_jts_trans[10]=mcu_to_jts.z_speed>>8;
	mcu_to_jts_trans[11]=mcu_to_jts.z_speed;
	mcu_to_jts_trans[12]=mcu_to_jts.acc_x>>24;
	mcu_to_jts_trans[13]=mcu_to_jts.acc_x>>16;
	mcu_to_jts_trans[14]=mcu_to_jts.acc_x>>8;
	mcu_to_jts_trans[15]=mcu_to_jts.acc_x;
	mcu_to_jts_trans[16]=mcu_to_jts.acc_y>>24;
	mcu_to_jts_trans[17]=mcu_to_jts.acc_y>>16;
	mcu_to_jts_trans[18]=mcu_to_jts.acc_y>>8;
	mcu_to_jts_trans[19]=mcu_to_jts.acc_y;
	mcu_to_jts_trans[20]=mcu_to_jts.acc_z>>24;
	mcu_to_jts_trans[21]=mcu_to_jts.acc_z>>16;
	mcu_to_jts_trans[22]=mcu_to_jts.acc_z>>8;
	mcu_to_jts_trans[23]=mcu_to_jts.acc_z;
	mcu_to_jts_trans[24]=mcu_to_jts.gyr_x>>24;
	mcu_to_jts_trans[25]=mcu_to_jts.gyr_x>>16;
	mcu_to_jts_trans[26]=mcu_to_jts.gyr_x>>8;
	mcu_to_jts_trans[27]=mcu_to_jts.gyr_x;
	mcu_to_jts_trans[28]=mcu_to_jts.gyr_y>>24;
	mcu_to_jts_trans[29]=mcu_to_jts.gyr_y>>16;
	mcu_to_jts_trans[30]=mcu_to_jts.gyr_y>>8;
	mcu_to_jts_trans[31]=mcu_to_jts.gyr_y;
	mcu_to_jts_trans[32]=mcu_to_jts.gyr_z>>24;
	mcu_to_jts_trans[33]=mcu_to_jts.gyr_z>>16;
	mcu_to_jts_trans[34]=mcu_to_jts.gyr_z>>8;
	mcu_to_jts_trans[35]=mcu_to_jts.gyr_z;
	mcu_to_jts_trans[36]=mcu_to_jts.voltage>>8;
	mcu_to_jts_trans[37]=mcu_to_jts.voltage;
	mcu_to_jts_trans[38]=UART_TAIL;

	Uart_DMA_Trans(Jetson_UART, mcu_to_jts_trans, MCU_TO_JTS_SIZE);
}


/*!
 * @brief 将数据下放到小车 v_x:mm/s; v_z:degree/s
 * @param  
 */
void Download_From_JTS(void){
	float target_v_line;//mm/s
	float target_v_z;//degree/s
	target_v_z=(float)((short)((jts_to_mcu.target_v_z_H<<8)+(jts_to_mcu.target_v_z_L)));
	target_v_line=(float)((short)(jts_to_mcu.target_v_x_H<<8)+(jts_to_mcu.target_v_x_L));
	Set_Car_Attitude(target_v_line,target_v_z); 
}
#endif
