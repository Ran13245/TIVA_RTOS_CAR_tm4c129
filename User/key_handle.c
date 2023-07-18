#include "bsp.h"

uint8_t tmp[5]={0x2c,0x12,0x01,0x01,0x5b};
extern bool up_begin;
void KEY0_CallBack(void){
    up_begin=1;
    printf_user(CONSOLE_UART,"key 0 pressed\r\n");
    tmp[3]=0x01;
    // Uart_DMA_Trans(K210_UART,tmp,5);
    printf_user(K210_UART,"%s",tmp);
}

void KEY1_CallBack(void){
    printf_user(CONSOLE_UART,"key 1 pressed\r\n");
    tmp[3]=0x00;
    Uart_DMA_Trans(K210_UART,tmp,5);
    Set_Car_Attitude(0,0);
}

void KEY2_CallBack(void){
    printf_user(CONSOLE_UART,"key 2 pressed\r\n");
}
