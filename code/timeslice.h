// 定义了时间片控制模块的接口。

#ifndef TIMESLICE_H_
#define TIMESLICE_H_

#include "zf_common_headfile.h"

// 定义普通PWM信号的最大最小占空比值（±3000）
#define pwm_duty_max         3000
#define pwm_duty_min        -3000

// 转向控制PWM信号的最大最小占空比值（±2000），用于限制电机或舵机的控制范围。
#define steer_pwm_duty_max   2000
#define steer_pwm_duty_min  -2000

// 声明基础速度和左右轮输出速度的全局变量
extern int base_speed_camera;
extern int base_speed_catheter;
extern int left_speed_output, right_speed_output;

// 用于控制摄像头和导管的基础速度以及左右轮的速度输出
void timeslice_init(void);
void timeslice_proc(void);

#endif
