/**
 * @file uart_handle.c
 * @author cbr (ran13245@outlook.com)
 * @brief UART输入的处理程序
 * @version 0.1
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "bsp.h"
#include "application.h"

void UartCallBack_USB(void){
#ifdef USB_UART
    Uart_DMA_Trans(CONSOLE_UART,uart_usb.receive,uart_usb.len);
    // float angle_lr,angle_ud;
    // angle_lr=((short)(uart_usb.receive[1]<<8)|(uart_usb.receive[2]))*0.1F;
    // angle_ud=((short)(uart_usb.receive[3]<<8)|(uart_usb.receive[4]))*0.1F;
    // Servo_Set_Degree_All(angle_lr,angle_ud);
    // printf_user(CONSOLE_UART,"%.2f,%.2f\r\n",angle_lr,angle_ud);
    // Uart_DMA_Trans(BLE_UART,&uart_usb.receive[1],uart_usb.len-2);//设置蓝牙用
#endif
}


void UartCallBack_BLE(void){
#ifdef BLE_UART
    Uart_DMA_Trans(CONSOLE_UART,uart_ble.receive,uart_ble.len);
#endif
}


void UartCallBack_JET(void){
#ifdef Jetson_UART
    // jts_to_mcu.target_v_z_H=uart_jetson.receive[5];
    // jts_to_mcu.target_v_z_L=uart_jetson.receive[6];
    // jts_to_mcu.target_v_x_H=uart_jetson.receive[3];
    // jts_to_mcu.target_v_x_L=uart_jetson.receive[4];
    Uart_DMA_Trans(CONSOLE_UART,uart_jetson.receive,uart_jetson.len);
    // Download_From_JTS();
    Set_target_servo_flag(uart_jetson.receive[1],(short)uart_jetson.receive[2]<<8|uart_jetson.receive[3]);

#endif
}


void UartCallBack_K210(void){
#ifdef K210_UART	
    uint8_t x=uart_k210.receive[2];
    int8_t y=uart_k210.receive[4];
    uint8_t flag_neg_y=uart_k210.receive[3];
    if(flag_neg_y)y=-y;
    uint8_t mode=uart_k210.receive[1];
static uint8_t last_mode=0;
    if(mode==0){
        Set_Car_Control(0,0,0);
    }
    if(mode==1&&car_control.spin_parameter.if_enable_interrupt){
        Set_Car_Control(x,y,0);
    }
    if(mode==2 && last_mode!=2){
        Set_Car_Control(0,0,-90);
    }
    last_mode=mode;
    Uart_DMA_Trans(CONSOLE_UART,uart_k210.receive,uart_k210.len);
#endif
}


void UartCallBack_OPENMV(void){
#ifdef OPENMV_UART

#endif
}
