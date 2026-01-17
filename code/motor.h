#ifndef MOTOR_H_
#define MOTOR_H_

#include "zf_common_headfile.h"


//// 左侧电机方向控制引脚定义
//#define DIR_L (P02_6)
//// 左侧电机PWM控制引脚定义
//#define PWM_L (ATOM0_CH7_P02_7)
//// 右侧电机方向控制引脚定义
//#define DIR_R (P02_4)
//// 右侧电机PWM控制引脚定义
//#define PWM_R (ATOM0_CH5_P02_5)

// 右侧电机方向控制引脚定义
#define DIR_L1              (P02_4)
// 右侧电机PWM控制引脚定义
#define PWM_L1              (ATOM0_CH5_P02_5)
// 左侧电机方向控制引脚定义
#define DIR_R1              (P02_6)
// 左侧电机PWM控制引脚定义
#define PWM_R1              (ATOM0_CH7_P02_7)

#define DIR_L2              (P21_2)
#define PWM_L2              (ATOM0_CH1_P21_3)
#define DIR_R2              (P21_4)
#define PWM_R2              (ATOM0_CH3_P21_5)


extern int pwm1, pwm2;

/**
 * @brief 电机初始化函数
 * @param 无
 * @return 无
 */
void motor_init(void);

/**
 * @brief 电机控制处理函数
 * @param duty1 左侧电机占空比参数
 * @param duty2 右侧电机占空比参数
 * @return 无
 */
void motor_proc(int duty1, int duty2);

#endif
