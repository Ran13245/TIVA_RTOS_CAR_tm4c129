#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "icm20602.h"
#include "hw_memmap.h"
#include "driverlib/i2c.h"
#include "main.h"
#include "config.h"
#include "user_filter.h"
#include "car_attitude.h"
#include "FreeRTOS.h"
#include "task.h"

_imu_data_raw imu_data_raw={0};
_imu_data_float imu_data={0};
_imu_solve imu_solve={0};
_imu_data_float imu_offset={0};
bool if_get_offset;
uint32_t imu_timestamp;

extern void delay_ms(uint32_t n);


// Acc Full Scale Range  +-2G 4G 8G 16G 
enum Ascale
{
    AFS_2G=0,  
    AFS_4G,
    AFS_8G,
    AFS_16G
};

// Gyro Full Scale Range +-250 500 1000 2000 Degrees per second
enum Gscale
{
    GFS_250DPS=0,   
    GFS_500DPS,
    GFS_1000DPS,
    GFS_2000DPS
};

void ICM20602_GetOffset(void){
    uint32_t i=0;
    uint32_t cnt=2000;
    float cnt_rec=1.0f/cnt;
    _imu_data_float tmp={0};
    for(i=0;i<cnt;i++){
        ICM20602_read_once();
        tmp.a_x+=imu_data.a_x;
        tmp.a_y+=imu_data.a_y;
        tmp.a_z+=imu_data.a_z-IMU_ONE_G;
        tmp.g_x+=imu_data.g_x;
        tmp.g_y+=imu_data.g_y;
        tmp.g_z+=imu_data.g_z;
        delay_ms(1);
    }
    imu_offset.a_x=cnt_rec*tmp.a_x;
    imu_offset.a_y=cnt_rec*tmp.a_y;
    imu_offset.a_z=cnt_rec*tmp.a_z;
    imu_offset.g_x=cnt_rec*tmp.g_x;
    imu_offset.g_y=cnt_rec*tmp.g_y;
    imu_offset.g_z=cnt_rec*tmp.g_z;
    if_get_offset=1;
}

/*!
 * @brief 初始化ICM20602
 * @author cbr
 * @param  void
 */
void init_ICM20602(void){
    /*  TEMP_DIS=0, SLEEP=0 */
    ICM20602_WriteByte(ICM20602_PWR_MGMT_1, 0x01);
    delay_ms(20);
    /* Enable Acc & Gyro */
    ICM20602_WriteByte(ICM20602_PWR_MGMT_2, 0x00);
    delay_ms(20);
    /* 176Hz set TEMP_OUT_L, DLPF=4 (Fs=1KHz):0x04*/
    ICM20602_WriteByte(ICM20602_CONFIG, 0x04);
    delay_ms(20);
    /*  SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
     *  Where INTERNAL_SAMPLE_RATE = 1 kHz */
    ICM20602_WriteByte(ICM20602_SMPLRT_DIV, 0x00);
    delay_ms(20);
    /* Average of 8 samples | Accelerometer Low Pass Filter 21.2Hz */
    ICM20602_WriteByte(ICM20602_ACCEL_CONFIG2, 0x14);
    delay_ms(20);
    ICM20602_SetAccRange(AFS_2G);
    delay_ms(20);
    ICM20602_SetGyroRange(GFS_250DPS);
    delay_ms(20);
}


/*****************************************************************************************************
 * @name: ICM20602_SetAccRange
 * @brief: Calculates Accelerometer resolution
 * @params: 1.Range: 0=+-2g,1=+-4g,2=+-8g,3=+-16g
 * @retval: none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void ICM20602_SetAccRange(uint8_t Range)
{
    uint8_t AccelRange=0;
    switch(Range)
    {
        case AFS_2G:
            imu_solve.acc_resolution = ACC_RESOLUTION_2G;
            AccelRange = AFS_2G;
            break;
        case AFS_4G:
            imu_solve.acc_resolution = ACC_RESOLUTION_4G;
            AccelRange = AFS_4G;
            break;
        case AFS_8G:
            imu_solve.acc_resolution = ACC_RESOLUTION_8G;
            AccelRange = AFS_8G;
            break;
        case AFS_16G:
            imu_solve.acc_resolution = ACC_RESOLUTION_16G;
            AccelRange = AFS_16G;
            break;
    }
    /* bit[4:3] 0=+-2g,1=+-4g,2=+-8g,3=+-16g, ACC_HPF=On (5Hz) */
    ICM20602_WriteByte(ICM20602_ACCEL_CONFIG, AccelRange<<3);
}

/*****************************************************************************************************
 * @name:
 * @brief: Calculates Gyro resolution
 * @params: none
 * @retval: none
 * @author: Wang Geng Jie
 *****************************************************************************************************/
void ICM20602_SetGyroRange(uint8_t Range)
{
    uint8_t GyroRange=0;
    switch(Range)
    {
        case GFS_250DPS:
            imu_solve.gyr_resolution = GYR_RESOLUTION_250DPS;
            GyroRange = GFS_250DPS;
            break;
        case GFS_500DPS:
            imu_solve.gyr_resolution = GYR_RESOLUTION_500DPS;
            GyroRange = GFS_500DPS;
            break;
        case GFS_1000DPS:
            imu_solve.gyr_resolution = GYR_RESOLUTION_1000DPS;
            GyroRange = GFS_1000DPS;
            break;
        case GFS_2000DPS:
            imu_solve.gyr_resolution = GYR_RESOLUTION_2000DPS;
            GyroRange = GFS_2000DPS;
            break;
    }
    ICM20602_WriteByte(ICM20602_GYRO_CONFIG, GyroRange<<3); // bit[4:3] 0=+-250d/s,1=+-500d/s,2=+-1000d/s,3=+-2000d/s
}


/*!
 * @brief 读取ICM20602
 * @author cbr
 * @attention 数据将会放到*mdpIMU.p___中
 * @param  void
 */
void ICM20602_read_once(void){
    uint8_t Buffer[14];
    //float Temperature=0;
#if (!TEST_BENCH)
    taskENTER_CRITICAL();
#endif
    ICM20602_ReadData(ICM20602_ACCEL_XOUT_H, Buffer, 14);
    imu_timestamp = xTaskGetTickCount();
#if (!TEST_BENCH)
    taskEXIT_CRITICAL();
#endif
    imu_data_raw.a_x = (Buffer[0]<<8) | Buffer[1];
    imu_data_raw.a_y = (Buffer[2]<<8) | Buffer[3];
    imu_data_raw.a_z = (Buffer[4]<<8) | Buffer[5];
    //Temperature = ((Buffer[6]<<8) | Buffer[7])/TEMP_SENSITIVITY + ROOM_TEMP_OFFSET;
    imu_data_raw.g_x = (Buffer[8]<<8) | Buffer[9];
    imu_data_raw.g_y = (Buffer[10]<<8) | Buffer[11];
    imu_data_raw.g_z = (Buffer[12]<<8) | Buffer[13];
    if(if_get_offset){
        //!imu方向有问题,转换一下
        imu_data.a_y=biquadFilterApply(&imu_biquadFilter_a_x, (float)imu_data_raw.a_x*imu_solve.acc_resolution*IMU_ONE_G)-imu_offset.a_y;
        imu_data.a_x=biquadFilterApply(&imu_biquadFilter_a_y,-(float)imu_data_raw.a_y*imu_solve.acc_resolution*IMU_ONE_G)-imu_offset.a_x;
        imu_data.a_z=biquadFilterApply(&imu_biquadFilter_a_z,(float)imu_data_raw.a_z*imu_solve.acc_resolution*IMU_ONE_G)-imu_offset.a_z;
        imu_data.g_y=biquadFilterApply(&imu_biquadFilter_g_x,(float)imu_data_raw.g_x*imu_solve.gyr_resolution)-imu_offset.g_y;
        imu_data.g_x=biquadFilterApply(&imu_biquadFilter_g_y,-(float)imu_data_raw.g_y*imu_solve.gyr_resolution)-imu_offset.g_x;
        imu_data.g_z=biquadFilterApply(&imu_biquadFilter_g_z,(float)imu_data_raw.g_z*imu_solve.gyr_resolution)-imu_offset.g_z;
        Car_Attitude_Yaw_Update(imu_data.g_z,0.001F*TASK_ITV_IMU);
    }
    else{
        imu_data.a_y=(float)imu_data_raw.a_x*imu_solve.acc_resolution*IMU_ONE_G;
        imu_data.a_x=-(float)imu_data_raw.a_y*imu_solve.acc_resolution*IMU_ONE_G;
        imu_data.a_z=(float)imu_data_raw.a_z*imu_solve.acc_resolution*IMU_ONE_G;
        imu_data.g_y=(float)imu_data_raw.g_x*imu_solve.gyr_resolution;
        imu_data.g_x=-(float)imu_data_raw.g_y*imu_solve.gyr_resolution;
        imu_data.g_z=(float)imu_data_raw.g_z*imu_solve.gyr_resolution;
    } 
}

//*****************************************************************
//
//! 读取ICM20602指定数量的字节
//! \param Address 目标ICM20602寄存器地址
//! \param Data 存储数据的数组指针
//! \param Num  读取的字节数
//! \return none
//
//*****************************************************************
void ICM20602_ReadData(uint8_t Address,uint8_t Data[],uint8_t Num){
    //先读低位，再读高位
    uint8_t data_i=0;
    //发送地址
    I2CMasterSlaveAddrSet(ICM20602_I2C_BASE, ICM20602_DEV_ADD, false);
    I2CMasterDataPut(ICM20602_I2C_BASE, Address);
    while(I2CMasterBusy(ICM20602_I2C_BASE));
    I2CMasterControl(ICM20602_I2C_BASE, 0x03);//start之后跟随send,状态空闲->发送  
    while(I2CMasterBusy(ICM20602_I2C_BASE));

    //接收第一个数据
    I2CMasterSlaveAddrSet(ICM20602_I2C_BASE, ICM20602_DEV_ADD, true);
    I2CMasterControl(ICM20602_I2C_BASE, 0x0b);//重复start之后接收，状态发送->接收
    while(I2CMasterBusy(ICM20602_I2C_BASE));
    Data[0] = I2CMasterDataGet(ICM20602_I2C_BASE);
    data_i+=1;
    Num-=1;

    //接收后续数据
    while(Num>1){
        I2CMasterControl(ICM20602_I2C_BASE, 0x09);//接收,状态接收->接收
        while(I2CMasterBusy(ICM20602_I2C_BASE));
        Data[data_i] = I2CMasterDataGet(ICM20602_I2C_BASE);
        data_i+=1;
        Num-=1;
    }

    //接收最后一个数据
    I2CMasterControl(ICM20602_I2C_BASE, 0x01);//非应答接收，状态接收->接收
    while(I2CMasterBusy(ICM20602_I2C_BASE));
    Data[data_i] = I2CMasterDataGet(ICM20602_I2C_BASE);

    //发送stop条件
    I2CMasterControl(ICM20602_I2C_BASE, 0x04);//stop条件,状态接收->空闲
    while(I2CMasterBusy(ICM20602_I2C_BASE));

    // if(I2CMasterErr(ICM20602_I2C_BASE) != I2C_MASTER_ERR_NONE){
    //     ICM20602_err_handle();
    // }
}


//*****************************************************************
//
//! 向ICM20602写一个字节
//! \param Address 目标ICM20602寄存器地址
//! \param Data 发送的数据,一个字节
//! \return none
//
//*****************************************************************
void ICM20602_WriteByte(uint8_t Address,uint8_t Data){
    //发送地址
    I2CMasterSlaveAddrSet(ICM20602_I2C_BASE, ICM20602_DEV_ADD, false);
    I2CMasterDataPut(ICM20602_I2C_BASE, Address);
    while(I2CMasterBusy(ICM20602_I2C_BASE));
    I2CMasterControl(ICM20602_I2C_BASE, 0x03);//start之后跟随send,状态空闲->发送
    while(I2CMasterBusy(ICM20602_I2C_BASE));

    //发送数据
    I2CMasterDataPut(ICM20602_I2C_BASE, Data);
    I2CMasterControl(ICM20602_I2C_BASE, 0x05);//发送之后stop,状态发送->空闲
    while(I2CMasterBusy(ICM20602_I2C_BASE));

    // if(I2CMasterErr(ICM20602_I2C_BASE) != I2C_MASTER_ERR_NONE){
    //     ICM20602_err_handle();
    // }
}


//*****************************************************************
//
//! 从ICM20602读一个字节
//! \param Address 目标ICM20602寄存器地址
//! \return 读到的数据,一个字节
//
//*****************************************************************
uint8_t ICM20602_ReadByte(uint8_t Address){
    uint8_t Data;
    //先发送地址
    I2CMasterSlaveAddrSet(ICM20602_I2C_BASE, ICM20602_DEV_ADD, false);
    I2CMasterDataPut(ICM20602_I2C_BASE, Address);
    while(I2CMasterBusy(ICM20602_I2C_BASE));
    I2CMasterControl(ICM20602_I2C_BASE, 0x03);//start之后跟随send,状态空闲->发送
    while(I2CMasterBusy(ICM20602_I2C_BASE));

    //读取一个数据
    I2CMasterSlaveAddrSet(ICM20602_I2C_BASE, ICM20602_DEV_ADD, true);
    I2CMasterControl(ICM20602_I2C_BASE, 0x03);//重复start之后非应答接收,状态发送->接收
    while(I2CMasterBusy(ICM20602_I2C_BASE));
    Data = I2CMasterDataGet(ICM20602_I2C_BASE);

    //发送stop条件
    I2CMasterControl(ICM20602_I2C_BASE, 0x04);//stop条件,状态接收->空闲
    while(I2CMasterBusy(ICM20602_I2C_BASE));

    //错误处理
    // if(I2CMasterErr(ICM20602_I2C_BASE) != I2C_MASTER_ERR_NONE){
    //     ICM20602_err_handle();
    // }
    return Data;
}

void ICM20602_err_handle(void){
    while(1){

    }
}
