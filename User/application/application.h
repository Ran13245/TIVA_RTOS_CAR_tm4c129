#ifndef __APPLICATION_H_
#define __APPLICATION_H_

#include "app_holder.h"

/*app_holder*/
#define HOLDER_PITCH_RANGE 90
/*舵机临界角,到达该角度时停车翻转舵机,该值是安装时舵机-180度所指的方向在小车坐标系中的角度值,表征舵机底座和小车的相对位置*/
#define SERVO_CRITICAL 180

void init_app(void);

#endif
