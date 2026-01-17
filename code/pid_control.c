#include "pid_control.h"

// 相机控制级联PID控制器实例
// 定义两个独立的PID结构体，分别保存左右轮的状态
cascade pid_left;  // 左轮专用
cascade pid_right; // 右轮专用

//转向的大脑 (只用到 steer_ring_pid 部分)
cascade pid_steer;

// 导管控制级联PID控制器实例
cascade pid_catheter;

// 级联PID输出PWM值
int32 cascade_pwmout = 0;

// 各环路PWM输出值
int32 speed_pwm_out = 0;//速度
int32 steer_pwm_out = 0;//转向
int32 angle_pwm_out = 0;//角度

/**
 * @brief 初始化相机控制级联PID参数
 * @param prt 指向cascade结构体的指针
 */
//初始设置
void cascade_pid_init_camera(cascade *prt)
{
    // 设置速度环PID参数
    prt->speed_ring_pid.kp = 1.2;
    prt->speed_ring_pid.ki = 0.5;
    prt->speed_ring_pid.kd = 0;

    // 设置转向环PID参数
    prt->steer_ring_pid.kp = 1.5;
    prt->steer_ring_pid.kp2 = 0;
    prt->steer_ring_pid.kd = 0.6;
    prt->steer_ring_pid.gkd = 0;//陀螺仪
}

/**
 * @brief 计算速度环PID输出
 * @param target 目标速度值
 * @param input 实际速度输入值
 * @param PID 指向cascade结构体的指针
 * @return 限幅后的PWM输出值
 */
//增量式pid计算，速度控制函数
int32 speed_pid_calc(int16 target, int16 input, cascade *PID)
{
    // 计算速度误差
    PID->speed_ring_pid.error = target - input;
    // 计算增量式PID输出
    PID->speed_ring_pid.pid_pwmout += PID->speed_ring_pid.ki * PID->speed_ring_pid.error +
                                      PID->speed_ring_pid.kp *
                                          (PID->speed_ring_pid.error - PID->speed_ring_pid.last_error);

    // 更新上次误差值
    PID->speed_ring_pid.last_error = PID->speed_ring_pid.error;
    // 限制PWM输出范围
    PID->speed_ring_pid.pid_pwmout = limit(PID->speed_ring_pid.pid_pwmout, pwm_duty_max, pwm_duty_min);

    return PID->speed_ring_pid.pid_pwmout;
}

/**
 * @brief 计算转向环PID输出
 * @param image_error 图像误差值
 * @param current_gyro_value 当前陀螺仪值
 * @param PID 指向cascade结构体的指针
 * @return 限幅后的转向PWM输出值
 */
//转向控制函数，核心速度控制算法
int32 steer_pid_calc(float image_error, float current_gyro_value, cascade *PID)
{
    float steer_value;

    // 计算转向控制量，包含比例项、非线性比例项、微分项和陀螺仪反馈项
    steer_value = (image_error * PID->steer_ring_pid.kp) +
                  (image_error * fabsf(image_error) * PID->steer_ring_pid.kp2) +
                  ((image_error - PID->steer_ring_pid.last_image_err) * PID->steer_ring_pid.kd) +
                  (current_gyro_value * PID->steer_ring_pid.gkd) / 100;

    // 更新上次图像误差值
    PID->steer_ring_pid.last_image_err = (int)image_error;

    return limit((int32)steer_value, pwm_duty_max, pwm_duty_min);
}

/**
 * @brief 计算角度环PID输出
 * @param target 目标角度值
 * @param input 实际角度输入值
 * @param PID 指向cascade结构体的指针
 * @return 限幅后的角度PWM输出值
 */
//角度控制函数（方向保持）
float angle_pid_calc(float target, float input, cascade *PID)
{
    // 计算角度误差并放大50倍
    PID->angle_ring_pid.error = (target - input) * 50;
    // 计算角度环PID输出
    PID->angle_ring_pid.pid_pwmout = PID->angle_ring_pid.kp * PID->angle_ring_pid.error +
                                     PID->angle_ring_pid.kd *
                                         (PID->angle_ring_pid.error - PID->angle_ring_pid.last_error);

    // 更新上次误差值
    PID->angle_ring_pid.last_error = PID->angle_ring_pid.error;
    // 限制PWM输出范围
    PID->angle_ring_pid.pid_pwmout = (float)limit((int32)PID->angle_ring_pid.pid_pwmout, pwm_duty_max, pwm_duty_min);

    return PID->angle_ring_pid.pid_pwmout;
}

/**
 * @brief PWM输出值限幅函数
 * @param duty 待限幅的PWM值
 * @param duty_1 最大限幅值
 * @param duty_2 最小限幅值
 * @return 限幅后的PWM值
 */
//限幅函数
int32 limit(int32 duty, int32 duty_1, int duty_2)
{
    if (duty > duty_1)
    {
        return duty_1;
    }
    else if (duty < duty_2)
    {
        return duty_2;
    }
    else
    {
        return duty;
    }
}
