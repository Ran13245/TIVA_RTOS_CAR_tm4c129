//*****************************************************************
//
//! ist8310.h - IST8310磁传感器驱动
//! \author cbr
//! \date 2023/06/20
//! \version 1.0 
//
//*****************************************************************

#ifndef _IST8310_H
#define _IST8310_H

#include <stdint.h>
#include <stdbool.h>

//寄存器
#define		IST8310_WAI		0x00
#define		IST8310_STAT1	0x02
#define		IST8310_DATAXL	0x03
#define		IST8310_DATAXH	0x04
#define		IST8310_DATAYL	0x05
#define		IST8310_DATAYH	0x06
#define		IST8310_DATAZL	0x07
#define		IST8310_DATAZH	0x08
#define		IST8310_STAT2	0x09
#define		IST8310_CNTL1	0x0a
#define		IST8310_CNTL2	0x0b
#define		IST8310_STR		0x0c
#define		IST8310_TEMPL	0x1c
#define		IST8310_TEMPH	0x1d
#define		IST8310_AVGCNTL	0x41
#define		IST8310_PDCNTL	0x42

//数据基地址,设备地址在main.h中
#define		IST8310_DATA_BASE	0x03


//STAT1 bits
#define		IST8310_STAT_DOR		0x02
#define		IST8310_STAT_DRDY	    0x01

//数据转换
#define     RESOLUTION        0.3F    //uT/LSB

struct _magnet_data_raw{
	short X;
	short Y;
	short Z;
};

struct _magnet_data_float{
    float x;
    float y;
    float z;
};

extern struct _magnet_data_raw magnet_raw;
extern struct _magnet_data_float magnet;

//核心函数

void init_ist8310(void);
void IST8310_read_once(void);

//i2c相关

void IST8310_ReadData(uint8_t Address,uint8_t Data[],uint8_t Num);
void IST8310_WriteDataByte(uint8_t Address,uint8_t Data);
uint8_t IST8310_ReadDataByte(uint8_t Address);

//杂项

void IST8310_err_handle(void);
void IST8310_skipped_handle(void);
// void init_drv_HardwareIIC(uint32_t ui32Base);//I2C初始化,当未提供I2C初始化函数时启用

#endif
