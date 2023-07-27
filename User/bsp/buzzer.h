#ifndef __BUZZER_H_
#define __BUZZER_H_

#include <stdint.h>

#define PWMCLK_FRQ 15000000

void Ser_Buzzer_Hz(float hz);
void Buzzer_Buzz(uint32_t start_time, uint32_t current_time);
void Buzzer_Trigger(void);

#endif
