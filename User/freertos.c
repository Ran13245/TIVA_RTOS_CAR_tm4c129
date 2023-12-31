/**
 * @file freertos.c
 * @author cbr (ran13245@outlook.com)
 * @brief FreeRTOS应用程序
 * @version 0.1
 * @date 2023-6-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
#include "main.h"

#include "bsp.h"
#include "application.h"

SemaphoreHandle_t semphr_uart_receive = NULL;
SemaphoreHandle_t semphr_buzzer_trigger = NULL;
QueueHandle_t queue_key = NULL;

static TaskHandle_t AppTaskCreate_Handle = NULL;
static TaskHandle_t TaskHandle_LEDBlink = NULL;
static TaskHandle_t TaskHandle_9axisService = NULL;
static TaskHandle_t TaskHandle_UartHandle = NULL;
static TaskHandle_t TaskHandle_CarAttitude = NULL;
static TaskHandle_t TaskHandle_VoltageUpdate = NULL;
static TaskHandle_t TaskHandle_DataUpload = NULL;
static TaskHandle_t TaskHandle_Key = NULL;
static TaskHandle_t TaskHandle_Buzzer = NULL;
static TaskHandle_t TaskHandle_Screen = NULL;

static void AppTaskCreate(void* pvParameters);
static void Task_LEDBlink(void* pvParameters);
static void Task_9axisService(void* pvParameters);
static void Task_UartHandle(void* pvParameters);
static void Task_CarAttitude(void* pvParameters);
static void Task_VoltageUpdate(void* pvParameters);
static void Task_DataUpload(void* pvParameters);
static void Task_Key(void* pvParameters);
static void Task_Buzzer(void* pvParameters);
static void Task_Screen(void* pvParameters);

/*!
 * @brief RTOS初始化,启动引导程序
 * @param  void
 * @return 是否成功启动
 */
BaseType_t RTOS_Init(void){

	return xTaskCreate(AppTaskCreate,"AppTaskCreate",512,NULL,1,&AppTaskCreate_Handle);
	
}

/*!
 * @brief RTOS引导程序
 * @param pvParemeters 任务参数,NULL
 */
void AppTaskCreate(void* pvParameters){
	BaseType_t xReturn = pdPASS;

	taskENTER_CRITICAL();//进入临界区

/*任务创建*/
	printf_user(CONSOLE_UART,"-Task-\r\n");

	xReturn = xTaskCreate(Task_9axisService,"9axisService",2048,NULL,3,&TaskHandle_9axisService);
	if(pdPASS == xReturn)
		printf_user(CONSOLE_UART,"9axisService\r\n");

	xReturn = xTaskCreate(Task_UartHandle,"UartHandle",512,NULL,7,&TaskHandle_UartHandle);
	if(pdPASS == xReturn)
		printf_user(CONSOLE_UART,"UartHandle\r\n");

	xReturn = xTaskCreate(Task_CarAttitude,"CarAttitude",512,NULL,5,&TaskHandle_CarAttitude);
	if(pdPASS == xReturn)
		printf_user(CONSOLE_UART,"CarAttitude\r\n");
	
	xReturn = xTaskCreate(Task_VoltageUpdate,"VoltageUpdate",128,NULL,2,&TaskHandle_VoltageUpdate);
	if(pdPASS == xReturn)
		printf_user(CONSOLE_UART,"VoltageUpdate\r\n");

	xReturn = xTaskCreate(Task_DataUpload,"DataUpload",512,NULL,6,&TaskHandle_DataUpload);
	if(pdPASS == xReturn)
		printf_user(CONSOLE_UART,"DataUpload\r\n");

	xReturn = xTaskCreate(Task_Key,"Key",128,NULL,6,&TaskHandle_Key);
	if(pdPASS == xReturn);
		printf_user(CONSOLE_UART,"Key\r\n");

	xReturn = xTaskCreate(Task_Buzzer,"Buzzer",128,NULL,1,&TaskHandle_Buzzer);
	if(pdPASS == xReturn);
		printf_user(CONSOLE_UART,"Buzzer\r\n");

	xReturn = xTaskCreate(Task_Screen,"Screen",1024,NULL,1,&TaskHandle_Screen);
	if(pdPASS == xReturn);
		printf_user(CONSOLE_UART,"Screen\r\n");

	xReturn = xTaskCreate(Task_LEDBlink,"LEDBlink",128,NULL,1,&TaskHandle_LEDBlink);
	if(pdPASS == xReturn);
		printf_user(CONSOLE_UART,"LEDBlink\r\n");

	printf_user(CONSOLE_UART,"-Task-\r\n");

/*信号量创建*/
	printf_user(CONSOLE_UART,"-Semaphore-\r\n");

	semphr_uart_receive = xSemaphoreCreateCounting(RX_QUEUE_SIZE,0);
	if(semphr_uart_receive != NULL)
		printf_user(CONSOLE_UART,"uart_receive\r\n");

	semphr_buzzer_trigger = xSemaphoreCreateBinary();
	if(semphr_buzzer_trigger != NULL)
		printf_user(CONSOLE_UART,"buzzer_trigger\r\n");

	printf_user(CONSOLE_UART,"-Semaphore-\r\n");
	
/*队列创建*/
	printf_user(CONSOLE_UART,"-Queue-\r\n");

	queue_key = xQueueCreate(1,1);
	if(queue_key != NULL)
		printf_user(CONSOLE_UART,"key\r\n");

	printf_user(CONSOLE_UART,"-Queue-\r\n");


	vTaskDelete(AppTaskCreate_Handle);
	taskEXIT_CRITICAL();//退出临界区
}

/*!
 * @brief LED闪烁程序
 * @param pvParameters 任务参数,NULL
 */
void Task_LEDBlink(void* pvParameters){
	for(;;){
		Set_LED(0,0,1);
		vTaskDelay(500);
		Set_LED(0,1,0);
		vTaskDelay(500);
		Set_LED(0,1,1);
		vTaskDelay(500);
		Set_LED(1,0,0);
		vTaskDelay(500);
		Set_LED(1,0,1);
		vTaskDelay(500);
		Set_LED(1,1,0);
		vTaskDelay(500);
		Set_LED(1,1,1);
		vTaskDelay(500);
	}
}



/*!
 * @brief 9轴读取
 * @param pvParameters 
 */
void Task_9axisService(void* pvParameters){
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = TASK_ITV_IMU;
	xLastWakeTime = xTaskGetTickCount();
	for(;;){
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		// IST8310_read_once();
		ICM20602_read_once();
	}
}


/*!
 * @brief 串口处理程序,处理上位机传回的数据
 * @param pvParameters 
 */
void Task_UartHandle(void* pvParameters){
	BaseType_t xReturn = pdPASS;
	for(;;){
		xReturn = xSemaphoreTake(semphr_uart_receive,portMAX_DELAY);
		if(pdTRUE == xReturn){
			UartRxSolve();
		}
	}
}

/*!
 * @brief 小车姿态更新
 * @param pvParameters 
 */
void Task_CarAttitude(void* pvParameters){
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = TASK_ITV_CAR;
	xLastWakeTime = xTaskGetTickCount();
	for(;;){
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
		Motor_Update_Input_All();
		Car_Attitude_Update_Input();
		Car_Control_Update_Input();
		Car_Control_Vitual_Update_Input();
		
		Car_Control_Vitual_Update_Output();
		Car_Control_Update_Output();
		Car_Attitude_Update_Output();
		Motor_Update_Output_All();

		App_Update_Holder_All();
	}

}

/*!
 * @brief 小车电压更新
 * @param pvParameters 
 */
void Task_VoltageUpdate(void* pvParameters){
	for(;;){
		Voltage_Update();
		vTaskDelay(3000);
	}
}

/*!
 * @brief 数据上传到上位机
 * @param pvParameters 
 */
void Task_DataUpload(void* pvParameters){
	TickType_t xLastWakeTime;
	const TickType_t xFrequency = TASK_ITV_UPLOAD;
	xLastWakeTime = xTaskGetTickCount();
	for(;;){
		vTaskDelayUntil( &xLastWakeTime, xFrequency );
#ifdef Jetson_UART
		Upload_To_JTS();
#endif
		car_attitude.updated=0;
		Car_Control_Upload();
	}
}

/*!
 * @brief 按键服务
 * @param pvParameters 
 */
void Task_Key(void* pvParameters){
	BaseType_t xReturn = pdPASS;
	uint8_t key;
	for(;;){
		xReturn = xQueueReceive(queue_key,&key,portMAX_DELAY);
		if(pdTRUE == xReturn){
			if(key==0)KEY0_CallBack();
			if(key==1)KEY1_CallBack();
			if(key==2)KEY2_CallBack();
		}
		vTaskDelay(50);
	}
}

/*!
 * @brief 蜂鸣器服务
 * @param pvParameters 
 */
void Task_Buzzer(void* pvParameters){
	BaseType_t xReturn = pdPASS;
	static uint32_t buzzer_start_time=0;
	for(;;){
		xReturn = xSemaphoreTake(semphr_buzzer_trigger,0);
		if(pdTRUE==xReturn){
			buzzer_start_time=xTaskGetTickCount();
		}
		Buzzer_Buzz(buzzer_start_time,xTaskGetTickCount());
		vTaskDelay(50);
	}
}

/*!
 * @brief 屏幕服务
 * @param pvParameters 
 */
void Task_Screen(void* pvParameters){
	screen.use_nonblocking=1;
	for(;;){
		OLED_Print_Service();
		vTaskDelay(50);
	}
}
