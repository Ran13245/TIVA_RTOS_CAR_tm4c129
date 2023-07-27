/**
 * @file main.c
 * @author cbr (ran13245@outlook.com)
 * @brief TIVA_RTOS Project
 * @version 0.0
 * @date 2023-06-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include <stdint.h>
#include <stdbool.h>

#include "main.h"
#include "bsp.h"
#include "application.h"
#include "tm4c129_it.h"
#include "testbench.h"


void delay_ms(uint32_t n)
{
	uint32_t count;
	count=USER_SYS_FREQ/1000;		
	SysCtlDelay(n*count/3);				
}


#if		TEST_BENCH
int main(void){
	Enter_Testbench();
}
#else
#include "FreeRTOS.h"
#include "task.h"
extern BaseType_t RTOS_Init(void);//创建任务,在freertos.c中实现

/*!
 * @brief 初始化硬件驱动包
 * @param  void
 */
static void BSP_Init(void){
	SysCtlClockFreqSet(SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN|SYSCTL_USE_PLL|SYSCTL_CFG_VCO_240,USER_SYS_FREQ);
	FPUEnable();
	FPULazyStackingEnable();
	IntPriorityGroupingSet(3);

	
	init_drv_GPIO();
	init_drv_UART();
	init_drv_HardwareI2C();
	init_drv_uDMA();
	init_drv_ADC();
	init_drv_PWM();
	OLED_Init();
	init_OLED_Print();
	init_motor();
	init_servo();
	Set_LED(1,1,1);
	delay_ms(2000);
	printf_user(CONSOLE_UART,"Init IMU\r\n");
	Set_LED(1,0,0);
	// init_ist8310();
	init_ICM20602();
	Set_LED(0,0,1);
	printf_user(CONSOLE_UART,"Get IMU Offset\r\n");
	ICM20602_GetOffset();
	Set_LED(0,1,0);
	init_filter();
	printf_user(CONSOLE_UART,"Pass\r\n");
	init_Car_Attitude();
	Set_Car_Attitude(0,0);
	init_Car_Contorl();
	init_Car_Control_Vitual();
	// BLE_AT_Set();
	delay_ms(200);
	init_drv_tim();
}

int main(void){
	BaseType_t xReturn = pdPASS;//

	BSP_Init();
	printf_user(CONSOLE_UART,"BSP Pass.\r\n");
	init_app();
	printf_user(CONSOLE_UART,"App Pass.\r\n");

	xReturn = RTOS_Init();
	
	if(pdPASS == xReturn){
		printf_user(CONSOLE_UART,"Launch RTOS\r\n");
		IntMasterEnable();
		portENABLE_INTERRUPTS();
		printf_user(CONSOLE_UART,"Enable Int\r\n");
		vTaskStartScheduler();
	}
	else {
		printf_user(CONSOLE_UART,"RTOS Failed!\r\n");
	}

	while(1){

	}
}
#endif
