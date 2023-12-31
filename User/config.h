/**
 * @file config.h
 * @author cbr (ran13245@outlook.com)
 * @brief 设置
 * @version 0.1
 * @date 2023-06-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "const.h"
/**
 * @brief 杂项
 * ****************************************************************************************************************************
 */
/*系统频率,仅作为计算参数,不能用来设置*/
#define         USER_SYS_FREQ          120000000
/*----testbench是否启用----*/
#define         TEST_BENCH      0
/*是否对IMU数据进行滤波*/
#define         USE_IMU_FILTER      1
/*滤波器参数*/
#define         FILTER_FREQ_IMU_AX     50.0F
#define         FILTER_FREQ_IMU_AY     50.0F
#define         FILTER_FREQ_IMU_AZ     50.0F
#define         FILTER_FREQ_IMU_GX     25.0F
#define         FILTER_FREQ_IMU_GY     25.0F
#define         FILTER_FREQ_IMU_GZ     25.0F
/*两种控制方式二选一*/
/*是否使用积分级控制方式*/
#define         USE_CAR_CONTROL         1
/*是否使用虚拟位置控制方式*/
#define         USE_VITUAL_CONTROL      0
/*硬件定时器中断周期,us*/
#define         TIM_ITV_US              20

/**
 * @brief 串口
 * ****************************************************************************************************************************
 */
/*设备串口,不用时注释掉*/
#if (!TEST_BENCH)
    #define         USB_UART         UART4_BASE
    // #define         BLE_UART         UART1_BASE
    // #define         Jetson_UART      UART0_BASE
    // #define         K210_UART        UART0_BASE
    // #define         OPENMV_UART      UART4_BASE
    /*波形显示串口*/
    // #define         WAVE_UART        USB_UART
#else 
    #define         USB_UART         UART0_BASE
    #define         BLE_UART         UART1_BASE
    // #define         Jetson_UART      UART0_BASE
    // #define         K210_UART        UART3_BASE
    // #define         OPENMV_UART      UART4_BASE

    /*波形显示串口*/
    #define         WAVE_UART        USB_UART
#endif

/*OLED模拟串口*/
#define         SCREEN_UART      0xCB

/*终端串口*/
#define         CONSOLE_UART     SCREEN_UART

/*串口波特率*/
#define         BAUD_RATE_UART0     115200
#define         BAUD_RATE_UART1     9600
#define         BAUD_RATE_UART2     115200
#define         BAUD_RATE_UART3     115200
#define         BAUD_RATE_UART4     115200

/*是否启用非阻塞print函数.启用后，每次print不得超过16个ASCII字符并要留意print间隔*/
#define         USE_NON_BLOKING_PUT     0

/*帧头帧尾*/
#define         UART_HEAD        '['
#define         UART_TAIL        ']'


/**
 * @brief 中断优先级,(0~7)<<5,留意FreeRTOS管理中断最低优先级=2
 * ****************************************************************************************************************************
 */

#if   TEST_BENCH
/*串口*/
#define         PRIORITY_UART0      (0<<5)
#define         PRIORITY_UART1      (0<<5)
#define         PRIORITY_UART2      (0<<5)
#define         PRIORITY_UART3      (0<<5)
#define         PRIORITY_UART4      (0<<5)


/*EXTI*/
#define         PRIORITY_PORTG      (0<<5)
#define         PRIORITY_PORTE      (0<<5)

/*TIM*/
#define         PRIORITY_TIM        (0<<5)

#else
/*串口*/
#define         PRIORITY_UART0      (3<<5)
#define         PRIORITY_UART1      (3<<5)
#define         PRIORITY_UART2      (3<<5)
#define         PRIORITY_UART3      (3<<5)
#define         PRIORITY_UART4      (3<<5)


/*EXTI*/
#define         PRIORITY_PORTG      (1<<5)
#define         PRIORITY_PORTE      (4<<5)

/*TIM*/
#define         PRIORITY_TIM        (1<<5)
#endif

/**
 * @brief FreeRTOS各任务执行间隔时间,ms
 * 
 */
#define         TASK_ITV_CAR        5
#define         TASK_ITV_IMU        5
#define         TASK_ITV_UPLOAD     50

/**
 * @brief 小车参数
 * ****************************************************************************************************************************
 */
//电机个数,4个或2个,2个时使用前轮
#define         USE_4_MOTOR             1

//是否使用四倍频
#define         USE_4_TIMES_ENCODER     1

//是否从IMU读得当前角速度
#define         V_DEGREE_FROM_IMU       1

//是否使用角速度PID
#define         V_ANGLE_PID             0

//每圈编码器数
#define         ENC_EVERY_CIRCLE        1560
//轮直径 mm
#define         WHEEL_DIR               67
//轮周长 mm
#define         WHEEL_PERIMETER         210.4867096F

//圈速=实际速度/周长=编码器速度/每圈编码器数
//编码器速度/实际速度=每圈编码器数/周长         !!长度mm!!
#define         V_REAL_TO_ENC           7.36388588F

/*电机方向*/
#define         LF_DIR                  0
#define         LR_DIR                  0
#define         RF_DIR                  1
#define         RR_DIR                  1

//左右轮距的一半(用IMU测算)        !!单位mm!!
#define         FRAME_W_HALF            115.0F

//前后轮轴距的一半
#define         FRAME_L_HALF            100.0F

/**
 * @brief 极限参数
 * ****************************************************************************************************************************
 */
//电机目标编码器速度最大值,编码器单位/s
#define         MAX_V_ENC               (3*ENC_EVERY_CIRCLE)
//电机最大实际速度,mm/s
#define         MAX_V_REAL              MAX_V_ENC/V_REAL_TO_ENC
//电机输出最大PWM占空比
#define         MAX_MOTOR_DUTY          0.85F
//最大角速度,degree/s
#define         MAX_V_ANGLE             120.0F       

//尺度变换,占空比的100%对应PID的1000,提高计算精度
#define         ZOOM_PID_TO_DUTY        0.001F

//增量式PID增量限幅
#define         LIMIT_INC_LF            0.5F*MAX_MOTOR_DUTY/ZOOM_PID_TO_DUTY     /*单位:占空比1/1000*/
#define         LIMIT_INC_LR            0.5F*MAX_MOTOR_DUTY/ZOOM_PID_TO_DUTY     
#define         LIMIT_INC_RF            0.5F*MAX_MOTOR_DUTY/ZOOM_PID_TO_DUTY
#define         LIMIT_INC_RR            0.5F*MAX_MOTOR_DUTY/ZOOM_PID_TO_DUTY
#define         LIMIT_INC_POS           MAX_V_REAL*0.5F                              /*单位:mm/s,直线速度*/
#define         LIMIT_INC_SPIN          MAX_V_ANGLE                               /*单位:degree/s,角速度*/
#define         LIMIT_INC_V_ANGLE       0.5F*MAX_V_ANGLE*FRAME_W_HALF*DEGREE_TO_RAD                              /*单位:mm/s,角速度补偿速度*/
#define         LIMIT_INC_VITUAL_X      0.0F        /*单位百分比,线速度*/
#define         LIMIT_INC_VITUAL_Y      0.0F        /*单位百分比,角速度*/

//位置式PID限幅,PWM占空比100%对应PID的1000
#define         LIMIT_POS_LF            (MAX_MOTOR_DUTY*1000)
#define         LIMIT_POS_LR            (MAX_MOTOR_DUTY*1000)
#define         LIMIT_POS_RF            (MAX_MOTOR_DUTY*1000)
#define         LIMIT_POS_RR            (MAX_MOTOR_DUTY*1000)
#define         LIMIT_POS_POS           MAX_V_REAL 
#define         LIMIT_POS_SPIN          MAX_V_ANGLE
#define         LIMIT_POS_V_ANGLE       MAX_V_ANGLE*FRAME_W_HALF*DEGREE_TO_RAD
#define         LIMIT_POS_VITUAL_X      100.0F
#define         LIMIT_POS_VITUAL_Y      100.0F

//位置式PID积分限幅
#define         LIMIT_ITGR_MAX          1000000000.0F
#define         LIMIT_ITGR_LF           LIMIT_ITGR_MAX
#define         LIMIT_ITGR_LR           LIMIT_ITGR_MAX
#define         LIMIT_ITGR_RF           LIMIT_ITGR_MAX
#define         LIMIT_ITGR_RR           LIMIT_ITGR_MAX
#define         LIMIT_ITGR_POS          LIMIT_ITGR_MAX
#define         LIMIT_ITGR_SPIN         LIMIT_ITGR_MAX
#define         LIMIT_ITGR_V_ANGLE      LIMIT_ITGR_MAX
#define         LIMIT_ITGR_VITUAL_X     LIMIT_ITGR_MAX
#define         LIMIT_ITGR_VITUAL_Y     LIMIT_ITGR_MAX


/**
 * @brief PID参数
 * ****************************************************************************************************************************
 */
#define         P_LF            0.05F
#define         P_LR            0.05F
#define         P_RF            0.05F
#define         P_RR            0.05F
#define         P_POS           1.0F    //位置
#define         P_SPIN          2.0F    //原地旋转
#define         P_V_ANGLE       11.5F    //角速度
#define         P_VITAUL_X      1.0F
#define         P_VITAUL_Y      1.0F

#define         I_LF            0.01F
#define         I_LR            0.01F
#define         I_RF            0.01F
#define         I_RR            0.01F
#define         I_POS           0.0F
#define         I_SPIN          0.0F
#define         I_V_ANGLE       0.15F
#define         I_VITAUL_X      0.0F
#define         I_VITAUL_Y      0.0F

#define         D_LF            0.0F
#define         D_LR            0.0F
#define         D_RF            0.0F
#define         D_RR            0.0F
#define         D_POS           0.0F
#define         D_SPIN          0.0F
#define         D_V_ANGLE       6.0F
#define         D_VITAUL_X      100.0F
#define         D_VITAUL_Y      0.0F


/**
 * @brief 舵机参数
 * ****************************************************************************************************************************
 */
#define SERVO_1_HALF_RANGE 180
#define SERVO_2_HALF_RANGE 180
