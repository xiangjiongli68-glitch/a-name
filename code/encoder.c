//编码器，既计数器/计速器
#include "encoder.h"
//00计数的数值存放处
int l_pulse, r_pulse;
/**
 * @brief 初始化编码器
 *
 * 该函数用于初始化系统中的两个编码器方向接口，配置相应的脉冲和方向引脚
 *
 * @param 无参数
 * @return 无返回值
 */
void encoder_init(void)
{
    // 初始化第一个编码器方向接口，配置脉冲和方向引脚
    encoder_dir_init(ENCODER_DIR1, ENCODER_DIR_PULSE1, ENCODER_DIR_DIR1);
    // 初始化第二个编码器方向接口，配置脉冲和方向引脚
    encoder_dir_init(ENCODER_DIR2, ENCODER_DIR_PULSE2, ENCODER_DIR_DIR2);
}

/**
 * @brief 编码器数据处理函数
 *
 * 该函数负责读取左右轮编码器的脉冲计数，并清除编码器的计数器
 * 为下一次计数做准备。左轮编码器计数直接获取，右轮编码器计数
 * 需要统一坐标系方向。
 *
 * @param void 无参数
 * @return void 无返回值
 */
void encoder_proc(void)
{
    l_pulse = encoder_get_count(ENCODER_DIR1);
    encoder_clear_count(ENCODER_DIR1);

    r_pulse = -encoder_get_count(ENCODER_DIR2);
    encoder_clear_count(ENCODER_DIR2);
}
