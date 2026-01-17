#include "motor.h"

int pwm1, pwm2;

/**
 * @brief 初始化电机控制模块
 * @param void 无参数
 * @return void 无返回值
 */
void motor_init(void)
{
    // PWM 通道初始化频率 17KHz 占空比初始为 0
    // 右侧 R1
    gpio_init(DIR_R1, GPO, GPIO_HIGH, GPO_PUSH_PULL);   // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_R1, 17000, 0);                         // PWM 通道初始化频率 17KHz 占空比初始为 0

    // 左侧 L1
    gpio_init(DIR_L1, GPO, GPIO_HIGH, GPO_PUSH_PULL);   // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_L1, 17000, 0);                         // PWM 通道初始化频率 17KHz 占空比初始为 0

    // 右侧 R2
    gpio_init(DIR_R2, GPO, GPIO_HIGH, GPO_PUSH_PULL);   // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_R2, 17000, 0);                         // PWM 通道初始化频率 17KHz 占空比初始为 0

    // 左侧 L2
    gpio_init(DIR_L2, GPO, GPIO_HIGH, GPO_PUSH_PULL);   // GPIO 初始化为输出 默认上拉输出高
    pwm_init(PWM_L2, 17000, 0);                         // PWM 通道初始化频率 17KHz 占空比初始为 0
}

/**
 * @brief 控制电机运行
 * @param duty1 左电机PWM占空比，正值为正转，负值为反转
 * @param duty2 右电机PWM占空比，正值为正转，负值为反转
 * @return void 无返回值
 */
void motor_proc(int duty1, int duty2)
{
    // ================= 控制左侧 (L1 & L2) =================
    if (duty1 >= 0) // 正转
    {
        // 设置方向：假设低电平为 DIR 输出逻辑
        gpio_set_level(DIR_L1, GPIO_HIGH);  // DIR输出高电平
        gpio_set_level(DIR_L2, GPIO_HIGH);

        // 设置PWM
        pwm_set_duty(PWM_L1, duty1);        // 占空比
        pwm_set_duty(PWM_L2, duty1);
    }
    else // 反转
    {
        // 设置方向
        gpio_set_level(DIR_L1, GPIO_LOW);   // DIR输出低电平
        gpio_set_level(DIR_L2, GPIO_LOW);

        // 设置PWM (取绝对值)
        pwm_set_duty(PWM_L1, (-duty1));    // 占空比
        pwm_set_duty(PWM_L2, (-duty1));
    }

    // ================= 控制右侧 (R1 & R2) =================
    if (duty2 >= 0) // 正转
    {
        // 设置方向
        gpio_set_level(DIR_R1, GPIO_LOW);
        gpio_set_level(DIR_R2, GPIO_LOW);

        // 设置PWM
        pwm_set_duty(PWM_R1, duty2);
        pwm_set_duty(PWM_R2, duty2);
    }
    else // 反转
    {
        // 设置方向
        gpio_set_level(DIR_R1, GPIO_HIGH);
        gpio_set_level(DIR_R2, GPIO_HIGH);

        // 设置PWM (取绝对值)
        pwm_set_duty(PWM_R1, (-duty2));
        pwm_set_duty(PWM_R2, (-duty2));
    }

    pwm1 = duty1;
    pwm2 = duty2;
}
