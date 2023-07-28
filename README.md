# TIVA_RTOS_CAR_tm4c129

## 项目概述

此项目是@Pansamic 所作tm4c1290ncpdt智能小车集成板的驱动代码，用于电赛控制类小车题。  
板载硬件设备有：

- TIVA tm4c1290ncpdt Cortex-M4微处理器
- TB6612电机驱动
- ICM20602惯性传感器
- IST8301磁传感器
- OLED屏幕接口
- 蓝牙模块焊盘

支持驱动4个直流电机、3个4Pin串口、1个Type-C串口、一个5V-2A Type-C供电接口、一个SPI OLED屏幕以及若干GPIO和5V、3.3V供电引脚

## Features

- FreeRTOS进行任务调度
- 串口使用环形缓冲队列、数据包长度队列、DMA、信号量等机制，实现：
  - 单次数据包传输最大长度254字节；
  - 4个串口同时传输；
  - 单数据包长度50字节下双向1000HZ包传输频率不丢包；
  - 设备与具体串口解耦，可以任意指定设备串口而不需改变设备回调函数
- IMU使用巴特沃斯二阶滤波，静置30分钟积分所得偏航角误差$\pm0.5\degree$
- 软件PWM设备链
- 重定向的`print`函数，可以用`printf_user(<uart>,<string with args>,...)`实现对特定串口的print
- OLED虚拟串口，使用`printf_user`指定该虚拟串口，可以print到OLED上显示。非阻塞。
- 分层的小车姿态控制系统，可以选择直接控制小车速度，或是指定目标点，或是指定路径半径及角度。
- 支持二维舵机云台，带有x-y平面内的鸡头稳定。
- 系统封装，易于上层程序调用

## 工程文件结构

- FreeRTOS：FreeRTOS库
- Lib：TIVA官方库
  - driverlib：基本驱动库
  - inc：寄存器库
  - sensorlib：传感器库，此处使用其提供的I2C驱动
  - utils：杂项，此处包含官方重定义的`print`函数(不支持多串口，未使用)
- Listings/Objects：编译输出
- Startup：启动引导文件
- User：用户文件夹
  - application：具体到赛题的应用，此处已包含二维云台驱动
  - bsp：board support package板级支持包，与FreeRTOS耦合的集成板驱动库
  - utils：杂项

## 主要API

整车API均在`User`文件夹下，具体到赛题的API由用户编写后放在`User/application`

- bsp.h：板级驱动包头文件
- car_attitude：小车姿态控制，
  - `Set_Car_Attitude(float v_line_target,float v_angle_target)`设置小车直线速度及角速度，需要在`config.h`中关闭`USE_CAR_CONTROL`
  - `car_attitude.yaw`小车偏航角
- car_control：小车积分级控制，需要在`config.h`中开启`USE_CAR_CONTROL`
  - `Set_Car_Control(float x, float y, float angle)`指定小车轨迹点，用法详见注释
  - `BIAS_LINE`宏表示PID计算距离时的偏置，用于加快响应和提高目标点附近的速度
  - `SPIN_INT_RATE`在超过目标值的该比例之后给上位机提供一个信号`IntEnableAck`
  - `LINE_INT_RATE`同上
- `config.h`：设置文件，各项参数见注释
- const.h：某些常数
- freertos.c：集中存放FreeRTOS的任务、信号量等
- FreeRTOSConfig.h：FreeRTOS的设置文件
- `key_handle`：三个按键的回调函数
- main.h：引脚定义
- protocol：各项传输协议
  - `Upload_Car_IntEnableAck(void)`给上位机提供信号`IntEnableAck`表示上次积分级控制轨迹已经执行完一定比例
  - `Upload_Car_OperateDoneAck(void)`给上位机提供信号`OperateDoneAck`表示上次积分级控制轨迹已经执行完毕
- testbench：用于隔离FreeRTOS的测试空间
- tim_handle：定时器的中断回调函数
- tm4c129_it：各种中断处理程序
- `uart_handle`：串口设备的处理回调程序

### 其他API

- `Set_LED(uint8_t R,uint8_t G, uint8_t B);`设置LED颜色
- `OLED_Print_Clear(void);`OLED清屏
- `Buzzer_Trigger(void);`蜂鸣器响
