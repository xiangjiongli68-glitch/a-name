#include "init.h"

/*
    定义并初始化了一个名为car的car_state结构体变量。
    目标速度设为500
    运行状态设为0（关闭状态）
    使能状态设为0（关闭状态）
    轨迹数组初始化为{1, 0}。
*/
car_state car =
{
    .target_speed =  60,
    .run = 0,
    .enable = 1,
    .track = {1, 0}
};

/**
 * @brief 初始化所有系统模块
 *
 * 该函数用于初始化车辆的所有硬件模块和系统组件，包括IMU、菜单、按键、电机、
 * Flash存储、蜂鸣器、摄像头、编码器、时间片、显示屏等外设，以及PIT定时器
 *
 * @param void 无参数
 * @return void 无返回值
 */
void all_init(void)
{
    imu660_init();
    keyy_init();
    mt9v03x_init();
    menu_init();
    motor_init();
    flash_init();
    buzzer_init();
    encoder_init();
    timeslice_init();
    ips200_show_init();
    car_state_init();
    pit_ms_init(CCU60_CH0, 1); //1ms
    pit_ms_init(CCU60_CH1, 10);//10ms
    pit_ms_init(CCU61_CH0, 10);//10ms
}

/**
 * @brief 初始化车辆状态
 *
 * 该函数用于初始化车辆的状态相关参数和控制器
 *
 * @param void 无参数
 * @return void 无返回值
 */
void car_state_init(void)
{
    // 初始化左轮 (虽然也赋了转向参数，但左轮只会用其中的速度参数)
    cascade_pid_init_camera(&pid_left);

    // 初始化右轮
    cascade_pid_init_camera(&pid_right);

    // 初始化转向 (虽然也赋了速度参数，但转向只会用其中的转向参数)
    cascade_pid_init_camera(&pid_steer);
    // 清空左轮历史
    pid_left.speed_ring_pid.last_error = 0;
    pid_left.speed_ring_pid.pid_pwmout = 0;

    // 清空右轮历史
    pid_right.speed_ring_pid.last_error = 0;
    pid_right.speed_ring_pid.pid_pwmout = 0;
    // 清空转向历史
    pid_steer.steer_ring_pid.last_image_err = 0;
    //ins.break_zone_id = 1;
}
