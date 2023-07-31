#include "config.h"
#include "main.h"
#include "servo.h"
#include "driverlib/pwm.h"
#include "hw_memmap.h"
#include "bsp_pwm.h"
#include "bsp_uart.h"

_servo servo_1;
_servo servo_2;

uint8_t servo_uart_buf[2][SERVO_BUS_SIZE]={0};

static void pBus_Servo_Send(_servo* servo,uint16_t Position){
    servo->bus_uart_conf.buf[0]=0xff;
    servo->bus_uart_conf.buf[1]=0xff;
    servo->bus_uart_conf.buf[2]=servo->bus_uart_conf.ID;//ID
    servo->bus_uart_conf.buf[3]=0x09;//length
    servo->bus_uart_conf.buf[4]=SERVO_BUS_COMMAND_INST_WRITE;//command
    servo->bus_uart_conf.buf[5]=SERVO_BUS_REG_ADD;//reg
    servo->bus_uart_conf.buf[6]=Position&(0xff);
    servo->bus_uart_conf.buf[7]=Position>>8;
    servo->bus_uart_conf.buf[8]=SERVO_BUS_para_time&(0xff);
    servo->bus_uart_conf.buf[9]=SERVO_BUS_para_time>>8;
    servo->bus_uart_conf.buf[10]=SERVO_BUS_para_speed&(0xff);
    servo->bus_uart_conf.buf[11]=SERVO_BUS_para_speed>>8;
    uint16_t checksum=0;
    int i=0;
    for(i=2;i<=11;i++)
        checksum += servo->bus_uart_conf.buf[i];
    servo->bus_uart_conf.buf[12]= ~ (checksum & 0xff);

    Uart_DMA_Trans(SERVO_BUS_UART,servo->bus_uart_conf.buf,SERVO_BUS_SIZE);
}

void init_servo(void){
    servo_1.type=SERVO_1_TYPE;
    servo_1.pwm_conf.PWM_BASE=PWM_SERVO_1_BASE;
    servo_1.pwm_conf.PWM_OUT=PWM_SERVO_1_OUT;
    servo_1.range_half=SERVO_1_HALF_RANGE;
    servo_1.bus_uart_conf.buf=servo_uart_buf[0];
    servo_1.bus_uart_conf.ID=1;
    servo_1.dir=SERVO_1_DIR;
    servo_1.angle_to_pos=((float)SERVO_1_HALF_CNT_RANGE)/SERVO_1_HALF_RANGE;
    servo_1.pos_offset=SERVO_1_CNT_OFFSET;

    servo_2.type=SERVO_2_TYPE;
    servo_2.pwm_conf.PWM_BASE=PWM_SERVO_2_BASE;
    servo_2.pwm_conf.PWM_OUT=PWM_SERVO_2_OUT;
    servo_2.range_half=SERVO_2_HALF_RANGE;
    servo_2.bus_uart_conf.buf=servo_uart_buf[1];
    servo_2.bus_uart_conf.ID=2;
    servo_2.dir=SERVO_2_DIR;
    servo_2.angle_to_pos=((float)SERVO_2_HALF_CNT_RANGE)/SERVO_2_HALF_RANGE;
    servo_2.pos_offset=SERVO_2_CNT_OFFSET;

    Set_Servo_Angle(&servo_1,0);
    Set_Servo_Angle(&servo_2,0);
}

void Set_Servo_Angle(_servo* servo, float angle){    
    angle*=servo->dir;
    if(servo->type == PWM){
        if(angle>servo->range_half)angle=servo->range_half;
        if(angle<-servo->range_half)angle=-servo->range_half;  
        float duty=SERVO_DUTY_MIN+((angle+servo->range_half)/(2.0F*servo->range_half))*(SERVO_DUTY_MAX-SERVO_DUTY_MIN);
        servo->angle=angle;
        Set_Duty(servo->pwm_conf.PWM_BASE,servo->pwm_conf.PWM_OUT,duty);
    }
    if(servo->type == BUS_UART){
        pBus_Servo_Send(servo,(angle+servo->range_half)*servo->angle_to_pos + servo->pos_offset);
    }
}


