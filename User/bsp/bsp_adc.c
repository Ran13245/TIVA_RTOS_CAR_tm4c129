#include <stdint.h>
#include <stdbool.h>
#include "driverlib/sysctl.h"
#include "driverlib/adc.h"
#include "hw_memmap.h"
#include "driverlib/gpio.h"
#include "bsp_adc.h"
#include "main.h"

float main_voltage=0;

/*!
 * @brief 初始化ADC
 * @param  
 */
void init_drv_ADC(void){
    // 使能时钟
	SysCtlPeripheralEnable(SYSCTL_PERIPH_ADC0);
	GPIOPinTypeADC(POWER_BASE,POWER_PIN);
	// 采样配置
	ADCSequenceConfigure(ADC0_BASE,3,ADC_TRIGGER_PROCESSOR,0);
	// 采样因子配置
	ADCHardwareOversampleConfigure(ADC0_BASE,64);
	// 采样序列配置
	ADCSequenceStepConfigure(ADC0_BASE,3,0,ADC_CTL_CH5|ADC_CTL_END);
	// 使能ADC
	ADCSequenceEnable(ADC0_BASE,3);
    //开始取样
    ADCProcessorTrigger(ADC0_BASE , 3);
}

/*!
 * @brief 对电源电压采样一次,数据更新到main_voltage中
 * @param  
 */
void Voltage_Update(void){
    if( ADCBusy(ADC0_BASE) == false )
	{
		// ADC FIFO
		uint32_t ADCvalue;
		ADCSequenceDataGet(ADC0_BASE,3,&ADCvalue);
		main_voltage = 11.0f * (float)ADCvalue/4095.0f*3.3f;
	}
	ADCProcessorTrigger(ADC0_BASE , 3);
}


