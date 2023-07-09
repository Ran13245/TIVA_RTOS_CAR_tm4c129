#include "bsp.h"
#include "testbench.h"

#if     TEST_BENCH

extern void delay_ms(uint32_t n);

void Enter_Testbench(void){
    SysCtlClockFreqSet(SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL|SYSCTL_CFG_VCO_240,USER_SYS_FREQ);
    SysCtlClockFreqSet(SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL|SYSCTL_CFG_VCO_240,USER_SYS_FREQ);
    FPUEnable();
	FPULazyStackingEnable();
	IntPriorityGroupingSet(3);
    init_drv_GPIO();
	init_drv_UART();
    delay_ms(2000);//等待小车完全停止
    test_print();
    test_print();
    // test_encoder();
    // test_wave();
    // test_pwm_output();
    // test_motor_input();
    // test_motor_pid();
    // test_imu();
    // test_communicate();
    // test_communicate();
    // test_car();
    // test_utils();
    while(1);

}

/*!
 * @brief 测试输出
 * @attention 相关参数:波特率
 * @param  
 */
void test_print(void){
    uint8_t test[7]={'t','e','s','t','2','\r','\n'};
    uint32_t uiBase=CONSOLE_UART;
    init_drv_uDMA();
    while(1){
        // printf_user(uiBase,"test1\r\n");
        // printf_user(CONSOLE_UART,"%d",uDMAChannelSizeGet( UDMA_PRI_SELECT| 9)); 
        Uart_DMA_Trans(uiBase, test, 7);
        delay_ms(500);
    }
}

/*!
 * @brief 测试波形显示
 * @attention 相关设置 波形显示串口
 * @param  
 */
void test_wave(void){
    while (1)
    {
		DataScope_Load(motor_LeftFront.EncSource);
		DataScope_Load(motor_LeftRear.EncSource);
		DataScope_Load(motor_RightFront.EncSource);
		DataScope_Load(motor_RightRear.EncSource);
		DataScope();
        delay_ms(200);
    }
}

/*!
 * @brief 编码器测试
 * @attention 相关参数:极限参数,是否四倍频,V_REAL_TO_ENC
 * @param  
 */
void test_encoder(void){
    while (1)
    {
        printf_user(CONSOLE_UART,"LF:%d  ",motor_LeftFront.EncSource);
        printf_user(CONSOLE_UART,"LR:%d  ",motor_LeftRear.EncSource);
        printf_user(CONSOLE_UART,"RF:%d  ",motor_RightFront.EncSource);
        printf_user(CONSOLE_UART,"RR:%d  ",motor_RightRear.EncSource);
        printf_user(CONSOLE_UART,"\r\n");
        delay_ms(200);
    }
}

/*!
 * @brief pwm输出测试
 * @param  
 */
void test_pwm_output(void){
    init_drv_PWM();
    while (1)
    {
        Set_Duty(PWM_LF_BASE,PWM_LF_OUT,0.2);
    }
    
}

/*!
 * @brief 电机状态更新测试
 * @param  
 */
void test_motor_input(void){
    init_motor();
    while (1)
    {
        Motor_Update_Input_All();
        printf_user(CONSOLE_UART,"LF:%.2f  ",motor_LeftFront.v_real);
        printf_user(CONSOLE_UART,"LR:%.2f  ",motor_LeftRear.v_real);
        printf_user(CONSOLE_UART,"RF:%.2f  ",motor_RightFront.v_real);
        printf_user(CONSOLE_UART,"RR:%.2f  ",motor_RightRear.v_real);
        printf_user(CONSOLE_UART,"\r\n");
        delay_ms(200);
    }
    
}

/*!
 * @brief 电机测试
 * @attention 相关设置:电机速度PID,电机方向,极限参数
 * @param  
 */
void test_motor_pid(void){
    init_drv_PWM();
    init_motor();
    Set_Car_Start();
    // Motor_Set_V_Enc_All(0,0,0,-1000);
    GPIOPinWrite(motor_RightRear.DirBase1,motor_RightRear.DirPin1,~(motor_RightRear.DirPin1));
    GPIOPinWrite(motor_RightRear.DirBase2,motor_RightRear.DirPin2,(motor_RightRear.DirPin2));
    Set_Duty(motor_RightRear.PWMBase,motor_RightRear.PWMOut,0.2);
    while (1)
    {   
        // Motor_Update_Input(&motor_RightRear);
        // Motor_Update_Output(&motor_RightRear);
        Motor_Update_Input_All();
        // Motor_Update_Output_All();
        DataScope_Load(motor_LeftFront.v_enc*0.1F);
		DataScope_Load(motor_LeftRear.v_enc*0.1F);
		DataScope_Load(motor_RightFront.v_enc*0.1F);
		DataScope_Load(motor_RightRear.v_enc*0.1F);
		DataScope();
        
        delay_ms(TASK_ITV_CAR);
    }
}

/*!
 * @brief 9轴测试
 * @param  
 */
void test_imu(void){
    init_drv_HardwareI2C();
    init_ICM20602();
    ICM20602_GetOffset();
    init_filter();
    // init_ist8310();
    _imu_data_float add={0};
    while (1)
    {
        // IST8310_read_once();
        ICM20602_read_once();
        // car_attitude.yaw=Kalman_Filter_z(car_attitude.yaw,imu_data.g_z,TASK_ITV_IMU*0.001);
        // printf_user(CONSOLE_UART,"ax:%.2f  ay:%.2f  az:%.2f  ",imu_data.a_x,imu_data.a_y,imu_data.a_z);
        printf_user(CONSOLE_UART,"gx:%.2f  gy:%.2f  gz:%.2f  ",imu_data.g_x,imu_data.g_y,imu_data.g_z);
        // printf_user(CONSOLE_UART,"mx:%.2f  my:%.2f  mz:%.2f  ",magnet.x,magnet.y,magnet.z);
        // printf_user(CONSOLE_UART,"yaw:%.2f  ",car_attitude.yaw);
        // add.a_x+=imu_data.a_x/TASK_ITV_IMU;
        // add.a_y+=imu_data.a_y/TASK_ITV_IMU;
        // add.a_z+=(imu_data.a_z-IMU_ONE_G)/TASK_ITV_IMU;
        // add.g_x+=imu_data.g_x/TASK_ITV_IMU;
        // add.g_y+=imu_data.g_y/TASK_ITV_IMU;
        // add.g_z+=imu_data.g_z/TASK_ITV_IMU;

        // DataScope_Load(add.a_x);
        // DataScope_Load(add.a_y);
        // DataScope_Load(add.a_z);
        // DataScope_Load(add.g_x);
        // DataScope_Load(add.g_y);
        // DataScope_Load(add.g_z);
        DataScope_Load(imu_data.a_x*100);
        DataScope_Load(imu_data.a_y*100);
        DataScope_Load((imu_data.a_z)*100);
        DataScope_Load(imu_data.g_x*100);
        DataScope_Load(imu_data.g_y*100);
        DataScope_Load(imu_data.g_z*100);
        DataScope();
        printf_user(CONSOLE_UART,"\r\n");
        delay_ms(TASK_ITV_IMU);
    }    
}

/*!
 * @brief 上位机通信测试
 * @attention 相关文件:uart_handle;相关设置:通信协议protocol
 * @param  
 */
void test_communicate(void){
    // init_drv_uDMA();
    // init_drv_ADC();
    while (1)
    {
        // Upload_To_JTS();
        UartRxSolve();
        delay_ms(TASK_ITV_UPLOAD);
    }
    
}

/*!
 * @brief 整车姿态测试
 * @attention 相关设置:转向PID,极限参数,小车机械参数
 * @param  
 */
void test_car(void){
    init_drv_PWM();
    init_motor();
    Motor_Set_V_Enc_All(0,0,0,0);
    Set_Car_Start();
    init_Car_Attitude();
    car_attitude.target_v_angle=5;
    while (1)
    {
        Car_Attitude_Update_Input();
        Car_Attitude_Update_Output();
        DataScope_Load(car_attitude.current_v_angle);
		DataScope();
        delay_ms(TASK_ITV_CAR);
    }
}


void test_utils(void){
    init_drv_ADC();
    while(1){
        Voltage_Update();
        printf_user(CONSOLE_UART,"%.2f\r\n",main_voltage);
        delay_ms(2000);
    }
    
}

#endif
