#ifndef __SERVO_H_
#define __SERVO_H_

#include <stdint.h>
#include <stdbool.h>

#define SERVO_1_DIR -1.0f
#define SERVO_2_DIR 1.0f

//0.5ms~2.5ms
#define SERVO_DUTY_MIN 0.19
#define SERVO_DUTY_MAX 0.95

/*-------------bus servo config----------------*/
#define SERVO_BUS_SIZE 13
#define SERVO_BUS_REG_ADD 42
#define SERVO_BUS_COMMAND_INST_WRITE 0x03
#define SERVO_BUS_para_time 0
#define SERVO_BUS_para_speed 1500

typedef enum __servo_type{
    PWM=0,
    BUS_UART
} _servo_type;

typedef struct __pwm_conf
{
    uint32_t PWM_BASE;
    uint32_t PWM_OUT;
}_pwm_conf;

typedef struct __bus_uart_conf
{
    uint8_t ID;
    uint8_t* buf;
}_bus_uart_conf;


typedef struct __servo
{
    _servo_type type;
    _bus_uart_conf bus_uart_conf;
    _pwm_conf pwm_conf;
    float range_half;/*范围的一半,degree,0~180*/
    float angle;/*当前角度，用来读取*/
    float dir;/*方向，+-1*/
    float angle_to_pos;/*角度到计数值的转化*/
    short pos_offset;/*计数值的偏移量*/
}_servo;

extern _servo servo_1;
extern _servo servo_2;

void init_servo(void);
void Set_Servo_Angle(_servo* servo, float angle);

#endif 
