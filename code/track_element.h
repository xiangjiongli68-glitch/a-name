#ifndef TRACK_ELEMENT_H_
#define TRACK_ELEMENT_H_
#define BLIND_TIME  25//设定的盲转时间
#include "zf_common_headfile.h"

extern int32_t WAVY_LINE;
extern int32_t BLANK_LINE;

extern uint8 angle_l, angle_r, island_l, island_r;
//extern double target_angle;
extern uint8 line_end_row;
void element_proc(void);

#endif
