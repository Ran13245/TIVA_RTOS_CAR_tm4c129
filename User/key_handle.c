#include "bsp.h"

uint8_t tmp[5]={0x2c,0x12,0x01,0x01,0x5b};
uint8_t to_nano[3]={0x5B,0x00,0x5D};

extern void delay_ms(uint32_t n);

extern bool up_begin;
void KEY0_CallBack(void){
    up_begin=1;
    printf_user(CONSOLE_UART,"key 0 pressed\r\n");
    tmp[3]=0x01;
    Uart_DMA_Trans(K210_UART,tmp,5);
    // printf_user(K210_UART,"%s",tmp);
}

void KEY1_CallBack(void){
    printf_user(CONSOLE_UART,"key 1 pressed\r\n");
    static uint8_t state=1;
    switch (state)
    {
        case 1:{
            to_nano[1]=1;
            printf_user(CONSOLE_UART,"1\r\n");
            state=2;
            break;
        }
        case 2:{
            to_nano[1]=2;
            printf_user(CONSOLE_UART,"2\r\n");
            state=3;
            break;
        }
        case 3:{
            to_nano[1]=3;
            printf_user(CONSOLE_UART,"3\r\n");
            state=4;
            break;
        }
        case 4:{
            to_nano[1]=4;
            printf_user(CONSOLE_UART,"4\r\n");
            state=5;
            break;
        }
        case 5:{
            to_nano[1]=5;
            printf_user(CONSOLE_UART,"5\r\n");
            state=6;
            break;
        }
        case 6:{
            to_nano[1]=6;
            printf_user(CONSOLE_UART,"6\r\n");
            state=7;
            break;
        }
        case 7:{
            to_nano[1]=7;
            printf_user(CONSOLE_UART,"7\r\n");
            state=8;
            break;
        }
        case 8:{
            to_nano[1]=8;
            printf_user(CONSOLE_UART,"8\r\n");
            state=1;
            break;
        }
    }
}

void KEY2_CallBack(void){
    printf_user(CONSOLE_UART,"key 2 pressed\r\n");
    Uart_DMA_Trans(Jetson_UART,to_nano,3);
}
