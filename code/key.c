#include "keyy.h"
#include "common_menu.h"

uint8 key_val, key_down, key_up, key_old;

// 定义显示模式变量，默认0（图像模式）
extern volatile uint8 display_mode = 0;

void keyy_init(void)
{
    gpio_init(KEY1, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(KEY2, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(KEY3, GPI, GPIO_HIGH, GPI_PULL_UP);
    gpio_init(KEY4, GPI, GPIO_HIGH, GPI_PULL_UP);
}

uint8 key_read(void)
{
    uint8 temp = 0;

    if (gpio_get_level(KEY1) == 0) temp = 1;
    if (gpio_get_level(KEY2) == 0) temp = 2;
    if (gpio_get_level(KEY3) == 0) temp = 3;
    if (gpio_get_level(KEY4) == 0) temp = 4;

    return temp;
}

void key_proc(void)
{
    key_val = key_read();                    
    key_down = key_val & (key_val ^ key_old);
    key_up  = ~key_val & (key_val ^ key_old); 
    key_old = key_val;                       

    // 如果没有按键按下，直接返回，节省CPU
        if (key_down == 0) return;
    //模式 0: 图像模式
        if (display_mode == 0)
        {
            // 在图像模式下，按下确认键(Key3)进入菜单模式
            if (key_down == 3)
            {
                display_mode = 1;       // 切换菜单标志位
                ips200_clear(); // 进菜单前清屏（防止图像残留）
            }
        }
        //模式 1: 菜单模式
        else
        {
            switch (key_down)
            {
                case 1:
                    Menu_upFuntion();   // 向上
                    break;
                case 2:
                    Menu_downFuntion(); // 向下
                    break;
                case 3:
                    Menu_enterFuntion(); // 确认
                    break;
                case 4:
                    //当前处于最顶层目录)
                    //且当前并未处于"修改数值"的状态 (number_box_select == 0)
                    if (key_menu_p->father->father == NULL && key_menu_p->number_box_select == 0)
                    {
                        display_mode = 0;// 切换回图像模式
                        ips200_clear(); // 退出前清屏
                    }
                    else
                    {
                        Menu_quitFuntion();  // 正常的返回上一级
                    }
                    break;
            }
        }
    }
