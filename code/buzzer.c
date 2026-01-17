#include "buzzer.h"

/**
 * @brief 蜂鸣器初始化函数
 *
 * 该函数用于初始化蜂鸣器所使用的GPIO引脚，配置为输出模式并设置初始状态
 *
 * @param void 无参数
 * @return void 无返回值
 */
void buzzer_init(void)
{
    gpio_init(BUZZER_PIN, GPO, GPIO_LOW, GPO_PUSH_PULL);//引脚 低电平 推挽输出
}

/**
 * @brief 蜂鸣器处理函数
 *
 * 该函数根据岛屿传感器和角度传感器的状态来控制蜂鸣器的开关
 * 当任意一个岛屿传感器或角度传感器被触发时，蜂鸣器开启
 * 否则蜂鸣器关闭
 *
 * @param void 无参数
 * @return void 无返回值
 */
void buzzer_proc(void)
{
//    // 检查岛屿传感器状态，如果左岛屿或右岛屿传感器被触发，则开启蜂鸣器
//    if (island_l || island_r)
//    {
//        gpio_set_level(BUZZER_PIN, GPIO_HIGH);
//    }
//    // 检查角度传感器状态，如果左角度或右角度传感器被触发，则开启蜂鸣器
//    else if (angle_l || angle_r)
//    {
//        gpio_set_level(BUZZER_PIN, GPIO_HIGH);
//    }
//    // 所有传感器都未被触发时，关闭蜂鸣器
//    else
//    {
//        gpio_set_level(BUZZER_PIN, GPIO_LOW);
//    }
}
