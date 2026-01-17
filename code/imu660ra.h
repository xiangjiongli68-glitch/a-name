#ifndef IMU660RA_H_
#define IMU660RA_H_

#include "zf_common_headfile.h"

void imu660_init(void);
void imu660_proc(void);

extern double total_angle, lv_gyro_z;

#endif
