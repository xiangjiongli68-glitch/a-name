//调度中心
#include "timeslice.h"

// 全局变量：左右轮目标速度和输出速度
int left_speed_target = 0, right_speed_target = 0;//左右轮目标速度，极其重要
int left_speed_output = 0, right_speed_output = 0;

// 全局变量：摄像头和导管基础速度
int base_speed_camera;
int base_speed_catheter;

/**
 * @brief 初始化时间片调度系统
 *
 * 该函数用于初始化操作系统的时间片调度机制，设置相关的时间片参数
 * 和调度器的初始状态，为后续的进程/线程时间片轮转调度做准备。
 *
 * @param void 无参数
 * @return void 无返回值
 */
void timeslice_init(void)
{
    
}

// 传感器计数器
uint8 sensor_counter;

/**
 * @brief 时序处理函数，用于控制车辆的运行状态和速度调节
 *
 * 该函数根据车辆运行状态，计算并输出相应的转向和速度控制信号
 * 当车辆运行时，根据摄像头误差和陀螺仪数据进行转向控制，
 * 同时根据目标速度和实际脉冲进行速度PID控制
 * 当车辆停止时，将速度设置为0并停止电机
 *
 * @param void 无参数
 * @return void 无返回值
 */
//车辆运行的主逻辑循环
void timeslice_proc(void)
{
    if (car.run == 1)
    {
        base_speed_camera = car.target_speed;

        // 计算转向PWM输出值，基于图像误差、陀螺仪Z轴数据和PID参数
        steer_pwm_out = steer_pid_calc(image_err, 0, &pid_steer);
        // 限制转向PWM输出在最大最小值范围内
        steer_pwm_out = limit(steer_pwm_out, steer_pwm_duty_max, steer_pwm_duty_min);

        // 第二步：将转向差值 叠加 到 基础速度
        // 左轮 = 基准 + 转向 (加速)
        // 右轮 = 基准 - 转向 (减速)
        // 这样就实现了差速转向
        left_speed_target = base_speed_camera + steer_pwm_out;
        right_speed_target = base_speed_camera - steer_pwm_out;
        //left_speed_target = (int16_t)((base_speed_camera - steer_pwm_out) * 1.001f);

        // 计算左右轮速度PID输出
        left_speed_output = speed_pid_calc((int16_t)left_speed_target, (int16_t)l_pulse, &pid_left);
        right_speed_output = speed_pid_calc((int16_t)right_speed_target, (int16_t)r_pulse, &pid_right);

        // 执行电机控制
        motor_proc(left_speed_output, right_speed_output);

    }
    else if (car.run == 0)
    {
        base_speed_camera = 0;

        // 停止电机运行
        motor_proc(base_speed_camera, base_speed_camera);
    }
}
