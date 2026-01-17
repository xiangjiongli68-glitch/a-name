
#include "imu660ra.h"

// total_angle累计总角度，
double total_angle = 0;

// 去零漂
// 通过采集初始样本数据来校准陀螺仪的零点偏移，提高测量精度

/*
 *变量作用：CALIBRATION_SAMPLES 设置为 100 次采样，用于 calibrateGyro() 函数中校准
 *目的：通过采集初始样本数据来校准陀螺仪的零点偏移，提高测量精度
 *工作原理：在 calibrateGyro() 函数中，循环采集 100 次陀螺仪数据计算 X、Y、Z 轴的平均值作为偏移量（gyroXOffset、gyroYOffset、gyroZOffset）后续使用时从原始数据中减去偏移量，得到准确的角速度值
 *采样间隔：每次采样间隔 20ms（system_delay_ms(20)），总校准时间约 2 秒   这是 IMU 传感器使用前的标准校准流程，用于消除传感器固有的零点漂移误差。
 */
float CALIBRATION_SAMPLES = 50 * 2;

// gyroX/Y/ZOffset及其副本用于存储X、Y、Z三轴陀螺仪的零漂偏移值，后续传感器校准做准备。
float gyroXOffset = 0;
float gyroXOffset_rcc = 0;

float gyroYOffset = 0;
float gyroYOffset_rcc = 0;

float gyroZOffset = 0;
float gyroZOffset_rcc = 0;

/**
 * @brief 陀螺仪校准函数
 *
 * 该函数通过采集多组陀螺仪和加速度计数据，计算并设置陀螺仪的零偏补偿值
 * 同时也计算加速度计的相关补偿值，用于后续的传感器数据校准
 *
 * @return 无返回值
 */
//初始校准函数
void calibrateGyro()
{
    imu660ra_get_acc();

    // 初始化陀螺仪和加速度计数据累加变量
    float sumX = 0, sunX_rcc = 0;
    float sumY = 0, sunY_rcc = 0;
    float sumZ = 0, sunZ_rcc = 0;

    // 采集校准样本数据
    for (int i = 0; i < CALIBRATION_SAMPLES; i++)
    {
        imu660ra_get_gyro();
        sumX += imu660ra_gyro_transition(imu660ra_gyro_x);
        sunX_rcc += imu660ra_acc_transition(imu660ra_acc_x);

        sumY += imu660ra_gyro_transition(imu660ra_gyro_y);
        sunY_rcc += imu660ra_acc_transition(imu660ra_acc_y);

        sumZ += imu660ra_gyro_transition(imu660ra_gyro_z);
        sunZ_rcc += imu660ra_acc_transition(imu660ra_acc_z);

        system_delay_ms(20);
    }

    // 计算陀螺仪各轴的平均偏移值，作为校准补偿值
    gyroXOffset = sumX / CALIBRATION_SAMPLES;
    gyroXOffset_rcc = sunX_rcc / CALIBRATION_SAMPLES;

    gyroYOffset = sumY / CALIBRATION_SAMPLES;
    gyroYOffset_rcc = sunY_rcc / CALIBRATION_SAMPLES;

    gyroZOffset = sumZ / CALIBRATION_SAMPLES;
    gyroZOffset_rcc = sunZ_rcc / CALIBRATION_SAMPLES;
}

/*
    KFP: 卡尔曼滤波器参数结构体
    x_last: 上一次的预测值（状态预测）
    x_mid: 中间预测值（先验估计）
    x_now: 当前输出值（后验估计）
    p_last: 上一次的协方差预测值
    p_mid: 中间协方差预测值
    p_now: 当前协方差值
    kg: 卡尔曼增益（Kalman Gain）
    q: 过程噪声协方差
    r: 测量噪声协方差
*/
typedef struct
{
    float LastP;
    float Now_P;
    float out;
    float Kg;
    float Q;
    float R;
} KFP; // Kalman Filter Parameter

#define GRAVITY 9.81 // GRAVITY: 重力加速度常数，值为 9.81 m/s?

/*
每个滤波器的初始设置：
初始预测值 x_last = 0.02
过程噪声 q = 0.001（较低的过程噪声，表示系统模型较可靠）
测量噪声 r = 0.543（相对较高的测量噪声，表示传感器数据不太可靠）
*/
KFP KFP_height_1 = {0.02, 0, 0, 0, 0.001, 0.543};
KFP KFP_height_2 = {0.02, 0, 0, 0, 0.001, 0.543};
KFP KFP_height_3 = {0.02, 0, 0, 0, 0.001, 0.543};

/**
 * @brief 卡尔曼滤波
 * @param kfp 滤波器参数结构体指针
 * @param input 输入数据
 * @return 滤波后的输出值
 */
//卡尔曼滤波，对数据进行过滤，消除噪声
float kalmanFilter_1(KFP *kfp, float input)
{
    float really_jiaodu = input; // 预测状态值

    kfp->Now_P = kfp->LastP + kfp->Q;                           // 预测协方差
    kfp->Kg = kfp->Now_P / (kfp->Now_P + kfp->R);               // 计算卡尔曼增益
    kfp->out = kfp->out + kfp->Kg * (really_jiaodu - kfp->out); // 更新状态估计
    kfp->LastP = (1 - kfp->Kg) * kfp->Now_P;                    // 更新状态协方差

    return kfp->out; // 返回滤波后的值
}

/**
 * @brief 计算输入数值的逆平方根（1/√x）
 * 使用快速逆平方根算法优化性能
 * @param x 输入数值
 * @return x的逆平方根值
 */
//构建3D姿态
static float invSqrt(float x)
{
    float halfx = 0.5f * x;
    float y = x;
    long i = *(long *)&y;
    i = 0x5f3759df - (i >> 1);
    y = *(float *)&i;
    y = y * (1.5f - (halfx * y * y));
    return y;
}

// 三维空间中坐标结构体
typedef struct
{
    float X;
    float Y;
    float Z;
} FLOAT_XYZ;

// 三维空间中的角度结构体
typedef struct
{
    float yaw; // 偏航角（绕Z轴旋转）
    float pit; // 俯仰角（绕Y轴旋转）
    float rol; // 翻滚角（绕X轴旋转）
} FLOAT_ANGLE;

// PID控制器参数
#define Kp 3.0f                // 比例增益
#define Ki 0.001f              // 积分增益
#define halfT 0.0005f          // 采样周期的一半，1ms-0.0005f 5ms-0.0025
#define RadtoDeg 57.295779513f // 弧度转角度

// 四元数的四个分量，用于表示物体的三维姿态
static float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
// 各轴的积分误差，用于PID控制器的积分项计算
static float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f;

extern unsigned char total_angle_clear_flag;

// Z轴陀螺仪角速度值
double lv_gyro_z;

/**
 * @brief 更新四元数并计算姿态角度
 * 使用Mahony算法融合陀螺仪和加速度计数据
 * @param Gyr_rad 陀螺仪数据（弧度制）
 * @param Acc_filt 滤波后的加速度计数据
 * @param Att_Angle 输出的姿态角度结构体
 */
void IMUupdate(FLOAT_XYZ *Gyr_rad, FLOAT_XYZ *Acc_filt, FLOAT_ANGLE *Att_Angle)
{
    float ax = Acc_filt->X;
    float ay = Acc_filt->Y;
    float az = Acc_filt->Z;
    float gx = Gyr_rad->X;
    float gy = Gyr_rad->Y;
    float gz = Gyr_rad->Z;

    // 归一化加速度计数据
    float norm = invSqrt(ax * ax + ay * ay + az * az);
    ax *= norm;
    ay *= norm;
    az *= norm;

    // 计算重力在估计姿态下的预期加速度计读数
    float vx = 2.0f * (q1 * q3 - q0 * q2);
    float vy = 2.0f * (q0 * q1 + q2 * q3);
    float vz = q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3;

    // 计算加速度计测量值与预期值之间的误差
    float ex = (ay * vz - az * vy);
    float ey = (az * vx - ax * vz);
    float ez = (ax * vy - ay * vx);

    // 积分误差项，用于消除稳态误差
    exInt += ex * Ki;
    eyInt += ey * Ki;
    ezInt += ez * Ki;

    // 应用PI控制器校正陀螺仪偏置
    gx += Kp * ex + exInt;
    gy += Kp * ey + eyInt;
    gz += Kp * ez + ezInt;

    // 四元数积分，预测新的姿态
    float q0_pred = q0 + (-q1 * gx - q2 * gy - q3 * gz) * halfT;
    float q1_pred = q1 + (q0 * gx + q2 * gz - q3 * gy) * halfT;
    float q2_pred = q2 + (q0 * gy - q1 * gz + q3 * gx) * halfT;
    float q3_pred = q3 + (q0 * gz + q1 * gy - q2 * gx) * halfT;

    // 归一化四元数，保持单位长度
    norm = invSqrt(q0_pred * q0_pred + q1_pred * q1_pred + q2_pred * q2_pred + q3_pred * q3_pred);
    q0 = q0_pred * norm;
    q1 = q1_pred * norm;
    q2 = q2_pred * norm;
    q3 = q3_pred * norm;

    // 从四元数计算偏航角
    Att_Angle->yaw = atan2f(2.0f * (q1 * q2 + q0 * q3),
                            q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3) *
                     RadtoDeg;
}

// 三个浮点型变量存储预处理后的陀螺仪X、Y、Z轴数据，初始值为0
float gyro_x_pretreatment = 0;
float gyro_y_pretreatment = 0;
float gyro_z_pretreatment = 0;
// 两个双精度变量存储计算得到的横滚角和俯仰角
double roll;
double pitch;
// 一个字节型标志位用于控制IMU660RA传感器的单次读取操作
unsigned char imu660ra_in_once_flag = 0;

/**
 * @brief 获取Z轴偏航角
 * 对陀螺仪数据进行预处理、滤波和姿态解算
 * @return Z轴偏航角度
 */
//获取当前的偏航角（车头朝向）
float GetFusedZAngle()
{
    // 减去预设的陀螺仪偏移值
    gyro_x_pretreatment = imu660ra_gyro_transition(imu660ra_gyro_x) - gyroXOffset;
    // 对三轴数据分别应用卡尔曼滤波器
    gyro_x_pretreatment = kalmanFilter_1(&KFP_height_1, imu660ra_gyro_transition(imu660ra_gyro_x));
    // 将滤波后的数据转换为double类型
    gyro_x_pretreatment = (float)((int)gyro_x_pretreatment);

    gyro_y_pretreatment = imu660ra_gyro_transition(imu660ra_gyro_y) - gyroYOffset;
    gyro_y_pretreatment = kalmanFilter_1(&KFP_height_2, imu660ra_gyro_transition(imu660ra_gyro_y));
    gyro_y_pretreatment = (float)((int)gyro_y_pretreatment);

    gyro_z_pretreatment = imu660ra_gyro_transition(imu660ra_gyro_z) - gyroZOffset;
    gyro_z_pretreatment = kalmanFilter_1(&KFP_height_3, imu660ra_gyro_transition(imu660ra_gyro_z));
    gyro_z_pretreatment = (float)((int)gyro_z_pretreatment);

    // 获取原始加速度计XYZ三轴数据
    // 偏移补偿：减去预设的加速度计偏移值
    FLOAT_XYZ acc = {
        .X = imu660ra_acc_transition(imu660ra_acc_x) - gyroXOffset_rcc,
        .Y = imu660ra_acc_transition(imu660ra_acc_y) - gyroYOffset_rcc,
        .Z = imu660ra_acc_transition(imu660ra_acc_z) - gyroZOffset_rcc};
    // 将预处理后的陀螺仪数据转换为弧度制
    FLOAT_XYZ gyro_rad = {
        .X = gyro_x_pretreatment * 0.0174532925f,
        .Y = gyro_y_pretreatment * 0.0174532925f,
        .Z = gyro_z_pretreatment * 0.0174532925f};
    // 控制IMU660RA传感器的单次读取操作
    if (imu660ra_in_once_flag == 0)
    {
        roll = (double)atan2f(acc.Y, acc.Z);                                  // X轴角度
        pitch = (double)atan2f(-acc.X, sqrtf(acc.Y * acc.Y + acc.Z * acc.Z)); // Y轴角度
        // 根据初始欧拉角计算对应的四元数
        q0 = (cos(roll / 2) * cos(pitch / 2));
        q1 = sin(roll / 2) * cos(pitch / 2);
        q2 = cos(roll / 2) * sin(pitch / 2);
        q3 = sin(roll / 2) * sin(pitch / 2);
        imu660ra_in_once_flag = 1;
    }

    /*
    IMUupdate 函数进行姿态更新
    角速度：gx_rad、gy_rad、gz_rad
    加速度：acc.x、acc.y、acc.z
    */
    FLOAT_ANGLE angle;
    IMUupdate(&gyro_rad, &acc, &angle); // 将弧度转换为角度

    lv_gyro_z = (double)gyro_rad.Z;

    return angle.yaw; // 偏航角度值
}

// 角度解包裹的结构体，主要用于解决角度值在 2π（或 360°）边界处发生跳变的问题。
typedef struct
{
    // 存储连续的、无跳变的总角度值,这是解包裹后的角度值，不会出现突然的大幅跳变
    float total_angle;
    // 上一次测量原始角度值,用于与当前角度值进行比较，检测是否发生跳变
    float prev_raw;
    // 是否是第一次处理，用于初始化处理流程，第一次处理时不需要比较前一个值
    uint8_t is_first;
} AngleUnwrapper;

/**
 * @brief 处理角度跳变
 * 解决角度值在±180度边界处的跳变问题，提供连续的角度输出
 * @param handler 角度解包器结构体指针
 * @param current_angle 当前角度值
 * @return 连续的角度值
 */
//将跳变的角度转换成连续的角度
float UnwrapAngle(AngleUnwrapper *handler, float current_angle)
{
    if (handler->is_first)
    {
        handler->total_angle = current_angle;
        handler->prev_raw = current_angle;
        handler->is_first = 0;
        return handler->total_angle;
    }

    float delta = current_angle - handler->prev_raw;

    if (delta > 180.0f)
    {
        delta -= 360.0f;
    }
    else if (delta < -180.0f)
    {
        delta += 360.0f;
    }

    handler->total_angle += delta;
    handler->prev_raw = current_angle;

    return handler->total_angle;
}

// Z轴角度解包裹器实例，用于处理角度跳变问题
AngleUnwrapper z_angle_handler = {0};

/**
 * @brief 获取连续的Z轴角度
 * @return 连续的Z轴角度值（无跳变）
 */
float GetContinuousZAngle(void)
{
    // 获取原始融合后的Z轴角度值
    float raw_angle = GetFusedZAngle();
    // 使用角度解包裹器处理角度跳变，返回连续的角度值
    return UnwrapAngle(&z_angle_handler, raw_angle);
}

// 存储陀螺仪偏航角的软件零点偏移量
static float yawSoftwareOffset = 0.0f;

/**
 * @brief 重置偏航角零点
 * 将当前角度设为新的零点参考
 */
void ResetYawZero(void)
{
    yawSoftwareOffset = GetContinuousZAngle();
}

/**
 * @brief 获取当前偏航角（相对于零点）
 * @return 返回当前陀螺仪偏航角值与软件零点偏移量的差值
 */
float GetCurrentYaw(void)
{
    return GetContinuousZAngle() - yawSoftwareOffset;
}

/**
 * @brief 陀螺仪初始化
 * 初始化IMU660RA传感器并进行零漂校准
 */
void imu660_init(void)
{
    imu660ra_init();
    calibrateGyro();
    // 陀螺仪进行零漂校准
}

/**
 * @brief 陀螺仪处理函数
 * 获取陀螺仪数据并计算总角度
 */
void imu660_proc(void)
{
    imu660ra_get_gyro();                   // 获取原始陀螺仪数据
    total_angle = (double)GetCurrentYaw(); // 获取相对于零点的偏航角
}
