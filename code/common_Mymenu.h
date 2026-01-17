/**********************************************************************************************
 代码用途: 多级菜单
**********************/
#ifndef __MENU_TASK_H_
#define __MENU_TASK_H_

#include <string.h>
#include "common_menu.h"
// #include "ST7789.h"
#include "zf_common_headfile.h"

void menu_init(void);
void menu_show_All(void);

// 步进加减
void MenuNumber_SetupCtrl_Plus(void);
void MenuNumber_SetupCtrl_Sub(void);

#define FOUR_BUTTON // 四个按键选择FOUR_BUTTON   五项按键选择FIVE_BUTTON

#ifdef FOUR_BUTTON
// 菜单控制部分
void Menu_upFuntion(void);      // 向上/加数据逻辑
void Menu_downFuntion(void);    // 向下/减数据逻辑
void Menu_enterFuntion(void);   // 确认/选中/反选逻辑
void Menu_quitFuntion(void);    // 返回/切换步进值逻辑
#endif



 #ifdef FIVE_BUTTON
 void Menu_upButton(void);           //给按键 上
 void Menu_downButton(void);         //给按键 下
 void Menu_leftButton(void);         //给按键 左
 void Menu_rightButton(void);        //给按键 右
 void Menu_midButton(void);          //给按键 中
 #endif

#endif
