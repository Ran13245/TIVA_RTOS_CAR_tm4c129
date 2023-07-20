
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#ifdef  WAVE_UART
void DataScope_Load(float data);
void DataScope(void);
#endif


#ifdef Jetson_UART

//MCU向jetson发送的数据个数
#define MCU_TO_JTS_SIZE 39

//MCU和jetson通讯协议
typedef struct __mcu_to_jts
{
    uint8_t flag;

    short x_speed;
    short y_speed;
    short z_speed;
    
    int32_t acc_x;
    int32_t acc_y;
    int32_t acc_z;

    int32_t gyr_x;
    int32_t gyr_y;
    int32_t gyr_z;

    uint32_t timestamp;
    short voltage;

}_mcu_to_jts;

typedef struct __jts_to_mcu
{
    uint8_t target_v_x_H;
    uint8_t target_v_x_L;
    uint8_t target_v_z_H;
    uint8_t target_v_z_L;
}_jts_to_mcu;


extern _jts_to_mcu jts_to_mcu;
extern _mcu_to_jts mcu_to_jts;

void Upload_To_JTS(void);
void Download_From_JTS(void);
#endif

void Upload_Car_IntEnableAck(void);
void Upload_Car_OperateDoneAck(void);

#endif
