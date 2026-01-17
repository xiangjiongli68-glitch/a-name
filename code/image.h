#ifndef IMAGE_H_
#define IMAGE_H_

#include "zf_common_headfile.h"

extern int32_t THRESHOLD;

#define Image_X 64
#define Image_Y 54

//常规情况黑0 白255
#define Black 0
#define White 255

extern uint8 bin_image[Image_Y][Image_X];
extern int far_white_count;
void image_proc(void);

#endif
