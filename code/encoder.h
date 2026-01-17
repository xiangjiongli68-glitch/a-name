#ifndef ENCODER_H_
#define ENCODER_H_

#include "zf_common_headfile.h"

// 定义编码器1左的硬件配置参数
#define ENCODER_DIR1        TIM6_ENCODER            // 使用 TIM6 定时器
#define ENCODER_DIR_PULSE1  TIM6_ENCODER_CH1_P20_3  // A相: P20_3
#define ENCODER_DIR_DIR1    TIM6_ENCODER_CH2_P20_0  // B相: P20_0


// 定义编码器2右的硬件配置参数
#define ENCODER_DIR2        TIM5_ENCODER            // 使用 TIM5 定时器
#define ENCODER_DIR_PULSE2  TIM5_ENCODER_CH1_P10_3  // A相: P10_3
#define ENCODER_DIR_DIR2    TIM5_ENCODER_CH2_P10_1  // B相: P10_1

// 外部声明左右轮脉冲计数变量
extern int l_pulse, r_pulse;

/**
 * @brief 编码器初始化函数
 * @param void 无参数
 * @return void 无返回值
 */
void encoder_init(void);

/**
 * @brief 编码器处理函数
 * @param void 无参数
 * @return void 无返回值
 */
void encoder_proc(void);

#endif
