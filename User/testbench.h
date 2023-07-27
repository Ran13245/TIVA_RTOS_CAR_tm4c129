#ifndef __TESTBENCH_H_
#define __TESTBENCH_H_

#if     TEST_BENCH

void Enter_Testbench(void);

void test_print(void);
void test_encoder(void);
void test_motor_pid(void);
void test_imu(void);
void test_communicate(void);
void test_car(void);
void test_wave(void);
void test_pwm_output(void);
void test_motor_input(void);
void test_utils(void);
void test_servo(void);
void test_oled(void);
#endif

#endif
