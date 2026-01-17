#ifndef __KEYY_H_
#define __KEYY_H_

#include "zf_common_headfile.h"

#define KEY1 (P11_3)
#define KEY2 (P11_2)
#define KEY3 (P20_7)
#define KEY4 (P20_6)

extern uint8 key_val;

// 0: 图像模式 (Image Mode)
// 1: 菜单模式 (Menu Mode)
extern volatile uint8_t display_mode;

void keyy_init(void);
uint8 key_read(void);
void key_proc(void);

#endif
