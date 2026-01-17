#ifndef PID_CONTROL_H_
#define PID_CONTROL_H_

#include "zf_common_headfile.h"

/**
 * 速度环PID控制结构体
 * 用于速度控制的PID参数和状态变量
 */
typedef struct
{
    float kp;         // 比例系数
    float ki;         // 积分系数
    float kd;         // 微分系数
    int16 error;      // 当前误差
    int16 last_error; // 上一次误差
    int32 pid_pwmout; // PID输出的PWM值
} Speed_Pid;

/**
 * 转向环PID控制结构体
 * 用于转向控制的PID参数和状态变量
 */
typedef struct
{
    int kp;             // 比例系数
    int kp2;            // 二次比例系数
    int kd;             // 微分系数
    int gkd;            // 陀螺仪微分系数
    int last_image_err; // 上一次图像误差
} Steer_Pid;

/**
 * 角度环PID控制结构体
 * 用于角度控制的PID参数和状态变量
 */
typedef struct
{
    float kp;         // 比例系数
    float ki;         // 积分系数
    float kd;         // 微分系数
    float error;      // 当前误差
    float last_error; // 上一次误差
    float pid_pwmout; // PID输出值
} Angle_Pid;

/**
 * 级联PID控制结构体
 * 包含速度环、转向环和角度环的PID控制器
 */
typedef struct
{
    Speed_Pid speed_ring_pid; // 速度环PID控制器
    Steer_Pid steer_ring_pid; // 转向环PID控制器
    Angle_Pid angle_ring_pid; // 角度环PID控制器
} cascade;

// 摄像头PID控制器实例
extern cascade pid_left;
extern cascade pid_right;

extern cascade pid_steer;
// 导管PID控制器实例
extern cascade pid_catheter;

// 各环路PWM输出变量
extern int32 cascade_pwmout; // 级联总PWM输出
extern int32 speed_pwm_out;  // 速度环PWM输出
extern int32 steer_pwm_out;  // 转向环PWM输出
extern int32 angle_pwm_out;  // 角度环PWM输出

/**
 * 限制函数
 * 将输入值限制在指定范围内
 * @param duty 待限制的值
 * @param duty_1 最小限制值
 * @param duty_2 最大限制值
 * @return 限制后的值
 */
int32 limit(int32 duty, int32 duty_1, int duty_2);

/**
 * 摄像头级联PID初始化函数
 * 初始化摄像头控制的级联PID参数
 * @param prt 级联PID结构体指针
 */
void cascade_pid_init_camera(cascade *prt);

/**
 * 速度PID计算函数
 * 根据目标值和输入值计算速度环PID输出
 * @param target 目标值
 * @param input 输入值
 * @param PID PID控制器指针
 * @return PID计算结果
 */
int32 speed_pid_calc(int16 target, int16 input, cascade *PID);

/**
 * 转向PID计算函数
 * 根据图像误差和陀螺仪值计算转向环PID输出
 * @param image_error 图像误差
 * @param current_gyro_value 当前陀螺仪值
 * @param PID PID控制器指针
 * @return PID计算结果
 */
int32 steer_pid_calc(float image_error, float current_gyro_value, cascade *PID);

/**
 * 角度PID计算函数
 * 根据目标角度和输入角度计算角度环PID输出
 * @param target 目标角度
 * @param input 输入角度
 * @param PID PID控制器指针
 * @return PID计算结果
 */
float angle_pid_calc(float target, float input, cascade *PID);

#endif
