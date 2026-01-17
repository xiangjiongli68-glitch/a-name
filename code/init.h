#ifndef INIT_H_
#define INIT_H_

#include "zf_common_headfile.h"

typedef struct
{
    uint8 camera;           // 摄像头循迹
    uint8 catheter;         // 惯导循迹
} track_mode;

typedef struct
{
    int target_speed;       // 目标速度
    int current_speed;      // 当前速度
    int prev_speed;         // 上一次速度

    uint8 run;              // 发车使能
    uint8 enable;

    track_mode track;
}car_state;

extern car_state car;
void all_init(void);
void car_state_init(void);

#endif
