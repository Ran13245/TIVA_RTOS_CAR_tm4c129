#include <stdint.h>
#include <stdbool.h>
#include "oled.h"
#include "oled_print.h"
#include "user_Queue.h"

#include "FreeRTOS.h"
#include "portable.h"

_screen screen={0};

char _screen_buffer[SCREEN_SIZE];

char _line[SCREEN_Y_SIZE][SCREEN_X_SIZE];

static void pOLED_Print_PutCharNonBlocking(char ch);
static void pOLED_Print_PutCharBlocking(char ch);
static void pOLED_Print_ScrollLine(void);
static void pOLED_Print_DisplayLine(uint8_t line_num);
static void pOLED_Print_NextCursor(bool jump_next_line);

/*!
 * @brief 初始化OLED打印显示
 * @param  
 */
void init_OLED_Print(void){
    uint8_t i=0;
    for(i=0;i<SCREEN_Y_SIZE;i++){
        screen.lines[i]=_line[i];
    }

    Queue_char_init(&screen.buffer,_screen_buffer,SCREEN_SIZE);
}

/*!
 * @brief 输出字符到屏幕，自动选择是否阻塞
 * @param ch 
 */
void OLED_Print_PutChar(char ch){
    if(screen.use_nonblocking)
        pOLED_Print_PutCharNonBlocking(ch);
    else 
        pOLED_Print_PutCharBlocking(ch);
}

/*!
 * @brief 显示服务，将字符从缓冲区中取出并处理换行
 * @param  
 */
void OLED_Print_Service(void){
    char ch;
    uint8_t i=0;
    while(Queue_char_count_remain(&screen.buffer)){
        ch=Queue_char_dequeue(&screen.buffer);
        if(ch=='\n'){
            pOLED_Print_NextCursor(true);
        }
        else{
            if(screen.cursor_y<SCREEN_Y_SIZE){
                screen.lines[screen.cursor_y][screen.cursor_x]=ch;
            }
            else{
                pOLED_Print_ScrollLine();
                screen.cursor_y=SCREEN_Y_SIZE-1;
                screen.lines[screen.cursor_y][screen.cursor_x]=ch;
            }
            pOLED_Print_NextCursor(false);
        }
    }

    for(i=0;i<SCREEN_Y_SIZE;i++){
        pOLED_Print_DisplayLine(i);
    }
}

/*!
 * @brief 清除显示
 * @param  
 */
void OLED_Print_Clear(void){
    uint8_t i=0;
    for(i=0;i<SCREEN_Y_SIZE;i++){
        pOLED_Print_ScrollLine();
    }
    screen.cursor_x=0;
    screen.cursor_y=0;
}


/*!
 * @brief 非阻塞式屏幕显示，依赖于OLED_Print_Service
 * @param ch 
 */
void pOLED_Print_PutCharNonBlocking(char ch){
    if(ch=='\r'||ch=='\0') return;/*过滤掉CR字符*/
    Queue_char_enqueue(&screen.buffer,ch);
}

/*!
 * @brief 非阻塞式显示，直接输出，不依赖于OLED_Print_Service
 * @param ch 
 */
void pOLED_Print_PutCharBlocking(char ch){
    pOLED_Print_PutCharNonBlocking(ch);
    OLED_Print_Service();
}

/*!
 * @brief 光标移动到下一个位置
 * @param jump_next_line 是否换行
 */
void pOLED_Print_NextCursor(bool jump_next_line){
    if(jump_next_line){
        screen.cursor_x=0;
        screen.cursor_y++;
    }
    else{
        screen.cursor_x++;
    }

    if(screen.cursor_x>=SCREEN_X_SIZE){
            screen.cursor_x=0;
            screen.cursor_y++;
    }
    if (screen.cursor_y>=SCREEN_Y_SIZE)
    {
        screen.cursor_y=SCREEN_Y_SIZE;
    }
}

/*!
 * @brief 屏幕滚动一行
 * @param  
 */
void pOLED_Print_ScrollLine(void){
    char* p_old_line=screen.lines[0];
    uint8_t i=0;
    for(i=0;i<SCREEN_Y_SIZE-1;i++){
        screen.lines[i]=screen.lines[i+1];
    }
    screen.lines[SCREEN_Y_SIZE-1]=p_old_line;
    
    for(i=0;i<SCREEN_X_SIZE;i++){
        screen.lines[SCREEN_Y_SIZE-1][i]=0;
    }
}

/*!
 * @brief 显示指定一行
 * @param line_num 待显示行数
 */
void pOLED_Print_DisplayLine(uint8_t line_num){
    uint8_t i=0;
    for(i=0;i<SCREEN_X_SIZE;i++){
        if(screen.lines[line_num][i]!=0)
            OLED_ShowChar(i*CHAR_WIDE,line_num*CHAR_HIGH,screen.lines[line_num][i]);
        else
            OLED_ShowChar(i*CHAR_WIDE,line_num*CHAR_HIGH,' ');
    }
}

