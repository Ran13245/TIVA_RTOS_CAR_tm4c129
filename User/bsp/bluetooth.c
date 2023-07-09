
#include "bluetooth.h"
#include "bsp_uart.h"
#include "driverlib/sysctl.h"

/*!
 * @brief 设置蓝牙模块,需要设置时调用一次即可
 * @param  
 */
void BLE_AT_Set(void){
    
    printf_user(BLE_UART,"AT+NAME=HC-09-ZRH");
    SysCtlDelay(200*SysCtlClockGet()/3000);	
    printf_user(BLE_UART,"AT+CLEAR");
}
