//*****************************************************************
//
//! ist8310.c - IST8310磁传感器驱动
//! \author cbr
//! \date 2023/06/20
//! \version 1.0 
//
//*****************************************************************

#include "main.h"
#include "ist8310.h"
#include "hw_memmap.h"
#include "driverlib/i2c.h"
#include "bsp_uart.h"

struct _magnet_data_raw magnet_raw={0,0,0};
struct _magnet_data_float magnet={0.0,0.0,0.0};

extern void delay_ms(uint32_t n);
//*****************************************************************
//
//! 初始化IST8310,注意需要先初始化I2C
//! \param none
//! \return none
//
//*****************************************************************
void init_ist8310(void){
    IST8310_WriteDataByte(IST8310_PDCNTL,0xc0);//启动
    delay_ms(20);
	IST8310_WriteDataByte(IST8310_AVGCNTL,0x24);//x,y,z均取16次平均
    delay_ms(20);
}


//*****************************************************************
//
//! IST8310传输过程中发生I2C总线错误的处理程序
//! \param none
//! \return none
//
//*****************************************************************
void IST8310_err_handle(void){
    I2CMasterIntClear(IST8310_I2C_BASE);
    printf_user(CONSOLE_UART,"IST8310 error\r\n");
    // init_drv_HardwareIIC(IST8310_I2C_BASE);
    while(1);   
}


//*****************************************************************
//
//! IST8310传输过程中丢失数据的处理程序
//! \param none
//! \return none
//
//*****************************************************************
void IST8310_skipped_handle(void){
    while(1);
}


/*!
 * @brief 读取IST8310数据一次,存储到 magnet 结构体中
 * @param  void
 */
void IST8310_read_once(void){
	// uint8_t state=0;
    uint8_t data[6];//ZH,ZL,YH,YL,XH,XL
	IST8310_WriteDataByte(IST8310_CNTL1,0x01);//enter single measurement mode
	// do{
	// 	state=IST8310_ReadDataByte(IST8310_STAT1);
	// 	if(state & IST8310_STAT_DOR)	IST8310_skipped_handle();
	// }while(!(state & IST8310_STAT_DRDY));
	
	IST8310_ReadData(IST8310_DATA_BASE,data,6);
	magnet_raw.X=(data[1]<<8) + data[0];
	magnet_raw.Y=(data[3]<<8) + data[2];
	magnet_raw.Z=(data[5]<<8) + data[4];

    magnet.x=(float)magnet_raw.X*RESOLUTION;
    magnet.y=(float)magnet_raw.Y*RESOLUTION;
    magnet.z=(float)magnet_raw.Z*RESOLUTION;
}


//*****************************************************************
//
//! 读取IST8310指定数量的字节
//! \param Address 目标IST8310寄存器地址
//! \param Data 存储数据的数组指针
//! \param Num  读取的字节数
//! \return none
//
//*****************************************************************
void IST8310_ReadData(uint8_t Address,uint8_t Data[],uint8_t Num){
    //先读低位，再读高位
    uint8_t data_i=0;
    //发送地址
    I2CMasterSlaveAddrSet(IST8310_I2C_BASE, IST8310_DEV_ADD, false);
    I2CMasterDataPut(IST8310_I2C_BASE, Address);
    while(I2CMasterBusy(IST8310_I2C_BASE));
    I2CMasterControl(IST8310_I2C_BASE, 0x03);//start之后跟随send,状态空闲->发送  
    while(I2CMasterBusy(IST8310_I2C_BASE));

    //接收第一个数据
    I2CMasterSlaveAddrSet(IST8310_I2C_BASE, IST8310_DEV_ADD, true);
    I2CMasterControl(IST8310_I2C_BASE, 0x0b);//重复start之后接收，状态发送->接收
    while(I2CMasterBusy(IST8310_I2C_BASE));
    Data[0] = I2CMasterDataGet(IST8310_I2C_BASE);
    data_i+=1;
    Num-=1;

    //接收后续数据
    while(Num>1){
        I2CMasterControl(IST8310_I2C_BASE, 0x09);//接收,状态接收->接收
        while(I2CMasterBusy(IST8310_I2C_BASE));
        Data[data_i] = I2CMasterDataGet(IST8310_I2C_BASE);
        data_i+=1;
        Num-=1;
    }

    //接收最后一个数据
    I2CMasterControl(IST8310_I2C_BASE, 0x01);//非应答接收，状态接收->接收
    while(I2CMasterBusy(IST8310_I2C_BASE));
    Data[data_i] = I2CMasterDataGet(IST8310_I2C_BASE);

    //发送stop条件
    I2CMasterControl(IST8310_I2C_BASE, 0x04);//stop条件,状态接收->空闲
    while(I2CMasterBusy(IST8310_I2C_BASE));

    if(I2CMasterErr(IST8310_I2C_BASE) != I2C_MASTER_ERR_NONE){
        IST8310_err_handle();
    }
}


//*****************************************************************
//
//! 向IST8310写一个字节
//! \param Address 目标IST8310寄存器地址
//! \param Data 发送的数据,一个字节
//! \return none
//
//*****************************************************************
void IST8310_WriteDataByte(uint8_t Address,uint8_t Data){
    //发送地址
    I2CMasterSlaveAddrSet(IST8310_I2C_BASE, IST8310_DEV_ADD, false);
    I2CMasterDataPut(IST8310_I2C_BASE, Address);
    while(I2CMasterBusy(IST8310_I2C_BASE));
    I2CMasterControl(IST8310_I2C_BASE, 0x03);//start之后跟随send,状态空闲->发送
    while(I2CMasterBusy(IST8310_I2C_BASE));

    //发送数据
    I2CMasterDataPut(IST8310_I2C_BASE, Data);
    I2CMasterControl(IST8310_I2C_BASE, 0x05);//发送之后stop,状态发送->空闲
    while(I2CMasterBusy(IST8310_I2C_BASE));

    if(I2CMasterErr(IST8310_I2C_BASE) != I2C_MASTER_ERR_NONE){
        IST8310_err_handle();
    }
}


//*****************************************************************
//
//! 从IST8310读一个字节
//! \param Address 目标IST8310寄存器地址
//! \return 读到的数据,一个字节
//
//*****************************************************************
uint8_t IST8310_ReadDataByte(uint8_t Address){
    uint8_t Data;
    //先发送地址
    I2CMasterSlaveAddrSet(IST8310_I2C_BASE, IST8310_DEV_ADD, false);
    I2CMasterDataPut(IST8310_I2C_BASE, Address);
    while(I2CMasterBusy(IST8310_I2C_BASE));
    I2CMasterControl(IST8310_I2C_BASE, 0x03);//start之后跟随send,状态空闲->发送
    while(I2CMasterBusy(IST8310_I2C_BASE));

    //读取一个数据
    I2CMasterSlaveAddrSet(IST8310_I2C_BASE, IST8310_DEV_ADD, true);
    I2CMasterControl(IST8310_I2C_BASE, 0x03);//重复start之后非应答接收,状态发送->接收
    while(I2CMasterBusy(IST8310_I2C_BASE));
    Data = I2CMasterDataGet(IST8310_I2C_BASE);

    //发送stop条件
    I2CMasterControl(IST8310_I2C_BASE, 0x04);//stop条件,状态接收->空闲
    while(I2CMasterBusy(IST8310_I2C_BASE));

    //错误处理
    if(I2CMasterErr(IST8310_I2C_BASE) != I2C_MASTER_ERR_NONE){
        IST8310_err_handle();
    }
    return Data;
}

// void init_drv_HardwareIIC(uint32_t ui32Base)
// {	
//     if(ui32Base == I2C0_BASE)
//     {	
//         SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C0);
//         SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
//         GPIOPinConfigure(GPIO_PB2_I2C0SCL);
//         GPIOPinConfigure(GPIO_PB3_I2C0SDA);
//         GPIOPinTypeI2C(GPIO_PORTB_BASE, GPIO_PIN_3);
//         GPIOPinTypeI2CSCL(GPIO_PORTB_BASE, GPIO_PIN_2);
//     }
//     I2CMasterInitExpClk(ui32Base, SysCtlClockGet(), true);
// }
