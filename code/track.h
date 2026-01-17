#ifndef TRACK_H_
#define TRACK_H_

#include "zf_common_headfile.h"

#define Image_X 64
#define Image_Y 54

extern int32_t search_way;

extern uint8 bin_image[Image_Y][Image_X];
extern uint8 mid_line[Image_Y];

extern uint8 l_edge, r_edge;
extern uint8 l_edge_up, r_edge_up, l_edge_down, r_edge_down;

extern float image_err;

int my_abs(int value);
int my_max(int x, int y);
void center_change(uint8 x1, uint8 y1, uint8 x2, uint8 y2);
void track_proc(void);
void image_err_proc(void);

#endif
