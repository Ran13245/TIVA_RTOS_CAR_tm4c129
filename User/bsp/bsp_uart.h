/**
 * @file bsp_uart.h
 * @author cbr (ran13245@outlook.com)
 * @brief 板级UART驱动
 * @attention 重定义了print,设置为非阻塞输出时每次print不得超过16个ASCII字符
 * @version 0.3
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __BSP_UART_H_
#define __BSP_UART_H_

#include "hw_memmap.h"
#include "config.h"


#include "user_RingBuf.h"
#include "user_Queue.h"

#include <stdbool.h>
#include <stdint.h>

#define BUFFER_SIZE  256
#define RX_QUEUE_SIZE 16

typedef struct __uart_device
{
    uint8_t buffer[BUFFER_SIZE];
    RingBuf_uint8_t ringbuf;
    uint8_t receive[BUFFER_SIZE];
    uint8_t _queue[RX_QUEUE_SIZE];
    Queue_uint8_t rx_len_queue;
    uint16_t rx_p;
    uint16_t tx_p;
    uint16_t len;
    bool received_head;
    bool receive_over;
}uart_device;

#ifdef USB_UART
extern uart_device uart_usb;
#endif
#ifdef BLE_UART
extern uart_device uart_ble;
#endif
#ifdef Jetson_UART
extern uart_device uart_jetson;
#endif
#ifdef K210_UART
extern uart_device uart_k210;
#endif
#ifdef OPENMV_UART
extern uart_device uart_openmv;
#endif

void init_drv_UART(void);
void UartRxIntHandler(uint32_t ui32Base);
void UartRxSolve(void);
void printf_user(uint32_t uiBase, const char *pcString, ...);
void error_uart(void);
void Uart_DMA_Trans(uint32_t uiBase, uint8_t *pcString, uint16_t length);
void error_uDMA(void);

//在uart_handle.h中定义
// #ifdef USB_UART
// void UartCallBack_USB(void);
// #endif
// #ifdef BLE_UART
// void UartCallBack_BLE(void);
// #endif
// #ifdef Jetson_UART
// void UartCallBack_JET(void);
// #endif
// #ifdef K210_UART
// void UartCallBack_K210(void);
// #endif
// #ifdef OPENMV_UART
// void UartCallBack_OPENMV(void);
// #endif

#endif
