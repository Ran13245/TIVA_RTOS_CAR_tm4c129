#ifndef __ICM20602_H_
#define __ICM20602_H_

#include <stdint.h>

/*寄存器*/
#define ICM20602_SELF_TEST_X_ACCEL  0x0D
#define ICM20602_SELF_TEST_Y_ACCEL  0x0E    
#define ICM20602_SELF_TEST_Z_ACCEL  0x0F
#define SELF_TEST_A                 0x10
#define ICM20602_XG_OFFS_USRH       0x13  // User-defined trim values for gyroscope; supported in MPU-6050?
#define ICM20602_XG_OFFS_USRL       0x14
#define ICM20602_YG_OFFS_USRH       0x15
#define ICM20602_YG_OFFS_USRL       0x16
#define ICM20602_ZG_OFFS_USRH       0x17
#define ICM20602_ZG_OFFS_USRL       0x18
#define ICM20602_SMPLRT_DIV         0x19
#define ICM20602_CONFIG             0x1A
#define ICM20602_GYRO_CONFIG        0x1B
#define ICM20602_ACCEL_CONFIG       0x1C
#define ICM20602_ACCEL_CONFIG2      0x1D  // Free-fall
#define ICM20602_LP_MODE_CFG        0x1E  // Free-fall
#define ICM20602_ACCEL_WOM_THR      0x1F  // Motion detection threshold bits [7:0]
#define MOT_DUR                     0x20  // Duration counter threshold for motion interrupt generation, 1 kHz rate, LSB = 1 ms
#define ZMOT_THR                    0x21  // Zero-motion detection threshold bits [7:0]
#define ZRMOT_DUR                   0x22  // Duration counter threshold for zero motion interrupt generation, 16 Hz rate, LSB = 64 ms
#define ICM20602_FIFO_EN            0x23
#define I2C_MST_CTRL                0x24   
#define I2C_SLV0_ADDR               0x25
#define I2C_SLV0_REG                0x26
#define I2C_SLV0_CTRL               0x27
#define I2C_SLV1_ADDR               0x28
#define I2C_SLV1_REG                0x29
#define I2C_SLV1_CTRL               0x2A
#define I2C_SLV2_ADDR               0x2B
#define I2C_SLV2_REG                0x2C
#define I2C_SLV2_CTRL               0x2D
#define I2C_SLV3_ADDR               0x2E
#define I2C_SLV3_REG                0x2F
#define I2C_SLV3_CTRL               0x30
#define I2C_SLV4_ADDR               0x31
#define I2C_SLV4_REG                0x32
#define I2C_SLV4_DO                 0x33
#define I2C_SLV4_CTRL               0x34
#define I2C_SLV4_DI                 0x35
#define ICM20602_FSYNC_INT          0x36
#define ICM20602_INT_PIN_CFG        0x37
#define ICM20602_INT_ENABLE         0x38
#define DMP_INT_STATUS              0x39  // Check DMP interrupt
#define ICM20602_INT_STATUS         0x3A
#define ICM20602_ACCEL_XOUT_H       0x3B
#define ICM20602_ACCEL_XOUT_L       0x3C
#define ICM20602_ACCEL_YOUT_H       0x3D
#define ICM20602_ACCEL_YOUT_L       0x3E
#define ICM20602_ACCEL_ZOUT_H       0x3F
#define ICM20602_ACCEL_ZOUT_L       0x40
#define ICM20602_TEMP_OUT_H         0x41
#define ICM20602_TEMP_OUT_L         0x42
#define ICM20602_GYRO_XOUT_H        0x43
#define ICM20602_GYRO_XOUT_L        0x44
#define ICM20602_GYRO_YOUT_H        0x45
#define ICM20602_GYRO_YOUT_L        0x46
#define ICM20602_GYRO_ZOUT_H        0x47
#define ICM20602_GYRO_ZOUT_L        0x48
#define EXT_SENS_DATA_00            0x49
#define EXT_SENS_DATA_01            0x4A
#define EXT_SENS_DATA_02            0x4B
#define EXT_SENS_DATA_03            0x4C
#define EXT_SENS_DATA_04            0x4D
#define EXT_SENS_DATA_05            0x4E
#define EXT_SENS_DATA_06            0x4F
#define ICM20602_SELF_TEST_X_GYRO   0x50
#define ICM20602_SELF_TEST_y_GYRO   0x51
#define ICM20602_SELF_TEST_z_GYRO   0x52
#define EXT_SENS_DATA_10            0x53
#define EXT_SENS_DATA_11            0x54
#define EXT_SENS_DATA_12            0x55
#define EXT_SENS_DATA_13            0x56
#define EXT_SENS_DATA_14            0x57
#define EXT_SENS_DATA_15            0x58
#define EXT_SENS_DATA_16            0x59
#define EXT_SENS_DATA_17            0x5A
#define EXT_SENS_DATA_18            0x5B
#define EXT_SENS_DATA_19            0x5C
#define EXT_SENS_DATA_20            0x5D
#define EXT_SENS_DATA_21            0x5E
#define EXT_SENS_DATA_22            0x5F
#define EXT_SENS_DATA_23            0x60
#define MOT_DETECT_STATUS           0x61
#define I2C_SLV0_DO                 0x63
#define I2C_SLV1_DO                 0x64
#define I2C_SLV2_DO                 0x65
#define I2C_SLV3_DO                 0x66
#define I2C_MST_DELAY_CTRL          0x67
#define ICM20602_SIGNAL_PATH_RESET  0x68
#define ICM20602_ACCEL_INTEL_CTRL   0x69
#define ICM20602_USER_CTRL          0x6A  // Bit 7 enable DMP, bit 3 reset DMP
#define ICM20602_PWR_MGMT_1         0x6B  // Device defaults to the SLEEP mode
#define ICM20602_PWR_MGMT_2         0x6C
#define DMP_BANK                    0x6D  // Activates a specific bank in the DMP
#define DMP_RW_PNT                  0x6E  // Set read/write pointer to a specific start address in specified DMP bank
#define DMP_REG                     0x6F  // Register in DMP from which to read or to which to write
#define DMP_REG_1                   0x70
#define DMP_REG_2                   0x71
#define ICM20602_FIFO_COUNTH        0x72
#define ICM20602_FIFO_COUNTL        0x73
#define ICM20602_FIFO_R_W           0x74
#define ICM20602_WHO_AM_I           0x75 // Should return 0x12


/*相关常量*/
#define IMU_ONE_G 9.793600082F
#define RAD_TO_DEG 57.29578f
#define TEMP_SENSITIVITY 326.8F
#define ROOM_TEMP_OFFSET 25.0F

#define ACC_RESOLUTION_2G   0.0001220703125F    //g/LSB
#define ACC_RESOLUTION_4G   0.000244140625F
#define ACC_RESOLUTION_8G   0.00048828125F
#define ACC_RESOLUTION_16G  0.0009765625F
// #define GYR_RESOLUTION_250DPS   0.007633587811F    //degree/s/LSB
#define GYR_RESOLUTION_250DPS   (0.007633587811F*1.5F)
#define GYR_RESOLUTION_500DPS   0.01526717562F
#define GYR_RESOLUTION_1000DPS  0.0304878056F
#define GYR_RESOLUTION_2000DPS  0.06097561121F



#define DisToAngle(Radius,Displace) ((Displace)/(Radius))
#define LinVelToAngVel(Radius,LinearVel)  ((LinearVel)/(Radius))
#define AngVelToLinVel(Radius,AngularVel) ((Radius)*(AngularVel))


/**
 * @name:Gyro_Kalman_t
 * @brief:a kalman filter for Gyro. This is a filter struct with a set of functions.
 * @member:
 *     1.Q_angle: process noise covariance for the accelerometer
 *     2.Q_bias: process noise covariance for the gyro bias
 *     3.R_measure: measurement noise covariance - this is actually the variance
 *         of the measurement noise
 *     4.angle: the angle calculated by the Kalman filter - part of the 2x1 state
 *         vector
 *     5.bias: the gyro bias calculated by the Kalman filter - part of the 2x1
 *         state vector
 *     6.P[2][2]: error covariance matrix - This is a 2x2 matrix
 *     7.K[2]: Kalman gain - This is a 2x1 vector
 *     8.y: angle difference - 1x1 vector
 *     9.S: estimate error - 1x1 vector
 * @note: none
 * */



typedef struct __imu_data_raw
{
    short g_x;
    short g_y;
    short g_z;
    short a_x;
    short a_y;
    short a_z;
}_imu_data_raw;

typedef struct __imu_data_float
{
    float g_x;//degree/s^2
    float g_y;
    float g_z;
    float a_x;//m/s^2
    float a_y;
    float a_z;
}_imu_data_float;

typedef struct __imu_solve
{
    float acc_resolution;
    float gyr_resolution;
}_imu_solve;

extern uint32_t imu_timestamp;
extern _imu_data_raw imu_data_raw;
extern _imu_data_float imu_data;
extern _imu_data_float imu_offset;
extern _imu_solve imu_solve;

void init_ICM20602(void);
void ICM20602_read_once(void);
void ICM20602_ReadData(uint8_t Address,uint8_t Data[],uint8_t Num);
void ICM20602_WriteByte(uint8_t Address,uint8_t Data);
uint8_t ICM20602_ReadByte(uint8_t Address);
void ICM20602_err_handle(void);
void ICM20602_SetAccRange(uint8_t Range);
void ICM20602_SetGyroRange(uint8_t Range);
void ICM20602_GetOffset(void);

#endif
