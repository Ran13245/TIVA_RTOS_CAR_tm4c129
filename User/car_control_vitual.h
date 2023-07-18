#include <stdint.h>
#include <stdbool.h>
#include "pid.h"

/*阻尼,值越大衰减越快,0~1*/
#define X_UPDATE_DAMP_RATIO  0.3F
#define Y_UPDATE_DAMP_RATIO  0.3F

#define X_RANGE 100.0F
#define Y_RANGE 100.0F
#define X_BIAS  10.0F
#define Y_BIAS  2.0F

typedef struct __car_control_vitual
{
    float x;
    float y;
    pid pid_x;
    pid pid_y;
    bool updated;
}_car_control_vitual;

extern _car_control_vitual car_control_vitual;

void init_Car_Control_Vitual(void);
void Set_Car_Control_Vitual(int x,int y);
void Car_Control_Vitual_Update_Input(void);
void Car_Control_Vitual_Update_Output(void);


