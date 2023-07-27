#ifndef __OLED_PRINT_H_
#define __OLED_PRINT_H_

#include <stdint.h>
#include <stdbool.h>
#include "user_Queue.h"

/*在OLED基本控制函数中的字符长宽*/
#define CHAR_WIDE   8
#define CHAR_HIGH   2

/*屏幕的横向、纵向分别可以容纳多少个字符*/
#define SCREEN_X_SIZE   16
#define SCREEN_Y_SIZE   4
#define SCREEN_SIZE (SCREEN_X_SIZE*SCREEN_Y_SIZE)

typedef struct __screen
{
    uint8_t cursor_x;
    uint8_t cursor_y;
    char* lines[SCREEN_Y_SIZE];
    bool use_nonblocking;
    Queue_char buffer;
}_screen;

extern _screen screen;

void init_OLED_Print(void);
void OLED_Print_PutChar(char ch);
void OLED_Print_Clear(void);
void OLED_Print_Service(void);

#endif
