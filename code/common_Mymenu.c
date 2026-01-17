/********************************* ******************************
 代码用途: 多级菜单
 ************************/
#include "common_Mymenu.h"
#include "keyy.h"
/* 宏定义菜单显示函数（需要自己定义） */
//-----------------------------------------------------------------------------------------------------------------

/* ***********************
 *  显示字符串
 *  格式    x   y   string
 * ***********************/
#define     menu_show_string(_x_, _y_, __string__)          ( ips200_show_string( (_x_), (_y_), (__string__) )  )
/* ***********************
 *  显示字符
 *  格式    x   y   char
 * **********************/
#define     menu_show_char(_x_, _y_, _char_)                ( ips200_show_char( (_x_), (_y_), (_char_) )        )
/* **********************
 *  显示整数
 *  格式    x   y   int     len
 * **********************/
#define     menu_show_int(_x_, _y_, _int_, _len_)           ( ips200_show_int( (_x_), (_y_), (_int_), (_len_) ) )
/* **********************
 *  显示浮点数
 *  格式    x   y   int     len
 * **********************/
#define     menu_show_float(_x_, _y_, _float_, _len_z_, _len_x_)    ( ips200_show_float( (_x_), (_y_), (_float_), (_len_z_), (_len_x_) ) )


#define     FONT_W                  (8)             //字体宽
#define     FONT_H                  (16)            //字体高
#define     SHOW_STAR_Y             (0)             //开始行
#define     COLS_SUM_LEN            (30)            //屏幕一行容纳字符数量              (屏幕像素 / 字体宽度)
#define     FOLDER_NAME_LEN         (15)            //名字长度限制
#define     FOLDER_NUMBER_LEN       (7)             //参数长度限制
#define     EVERY_FOLDER_NUMBER     (10)            //每页文件数量限制

#define SETUP_LEN               (7)
#define SETUP_NUMBER_LEN        (5+2)
//步进值
static float SetupNumber[SETUP_LEN] = {0.001, 0.01, 0.1, 1, 10, 100, 1000};
static uint8_t SetupIndex = 3;

//菜单头节点
Folder_Menu myMenu;
//菜单按键控制索引指针
Folder_Menu *key_menu_p = NULL;

/* ----------------------------手动创建参数---------------------------- */


//extern int test1;


/* ----------------------------手动创建参数---------------------------- */


//一、初始化与系统搭建
//函数：my_create_Menus(void) menu_init(void)
static void my_create_Menus(void)
{
    /* -------------------------菜单内容在下面添加------------------------ */
//   Folder_Menu *folder1 = dynamicCreate_Menu_Folder(&myMenu, "folder1");//创建文件夹

//    //创建二级菜单文件夹
//    dynamicCreate_Menu_Folder(folder1, "folder4");
//
//
//    //在文件夹中创建文件
//    dynamicCreate_Menu_LimitNumberBox(folder1, "num1", &test1, int_Box, -100, 200);

    /* ----------------------------在上面添加---------------------------- */
}

//---------------------------------------以下可以无视-----------------------------------------------

/**
 * @brief   菜单初始化
 * @param   
 * @return  void
 */
//这个也是初始化
void menu_init(void)
{
    //菜单头节点初始化(不用动)
    myMenu.father = NULL;
    myMenu.son_first = NULL;
    myMenu.next_brother = NULL;
    myMenu.last_brother = NULL;
    myMenu.name = "<<Menu>>";
    myMenu.sons_Count = 0;
    myMenu.No = 0;
    myMenu.kind = Normal_Folder;

    my_create_Menus();

    //下面是按键指针初始化(不用动)
    if(myMenu.son_first != NULL)
    key_menu_p = myMenu.son_first;
    //成员初始化
    All_Folder_Menu_Init(&myMenu);
}


//路径显示
//二、屏幕数据显示 函数：menu_show_All(void) Mymenu_show_title(void) Mymenu_show_Setup(void)
//Mymenu_show_task(void) Mymenu_show_key(void) Mymenu_show_data(void)
//在屏幕左上角显示当前所在的文件夹名字
static void Mymenu_show_title(void)
{
    char tmpchar[COLS_SUM_LEN-SETUP_NUMBER_LEN+1];
    for(int i = 0; i < COLS_SUM_LEN-SETUP_NUMBER_LEN+1; i++)
        tmpchar[i] = ' ';
    sprintf(tmpchar, "%s/", key_menu_p->father->name);
    tmpchar[strlen(tmpchar)] = ' ';
    tmpchar[COLS_SUM_LEN-SETUP_NUMBER_LEN] = '\0';
    menu_show_string(0, SHOW_STAR_Y, tmpchar);
}

//显示步进值
static void Mymenu_show_Setup(void)
{
    char tmpchar[SETUP_NUMBER_LEN+1];
    for(int i = 0; i < SETUP_NUMBER_LEN+1; i++)
        tmpchar[i] = ' ';
    if(SetupNumber[SetupIndex] < 1)
        sprintf(tmpchar, "<%.3f>", SetupNumber[SetupIndex]);
    else
        sprintf(tmpchar, "<%.0f>", SetupNumber[SetupIndex]);
    tmpchar[strlen(tmpchar)] = ' ';
    tmpchar[SETUP_NUMBER_LEN] = '\0';
    menu_show_string(COLS_SUM_LEN*FONT_W - FONT_W*SETUP_NUMBER_LEN, SHOW_STAR_Y, tmpchar);
}

//将所有子项的名字打印在屏幕上
static void Mymenu_show_task(void)
{
    char tmpchar[FOLDER_NAME_LEN+1];
    Folder_Menu *son_p = key_menu_p->father->son_first;
    for(int i = 1; i <= key_menu_p->father->sons_Count; i++)
    {
        for(int j = 0; j < FOLDER_NAME_LEN+1; j ++) {
            tmpchar[j] = ' ';
        }
        if(son_p->kind == Normal_Folder) {
            sprintf(tmpchar, "%s\\[%d]", son_p->name, son_p->sons_Count);
            tmpchar[strlen(tmpchar)] = ' ';
            tmpchar[FOLDER_NAME_LEN] = '\0';
        }
        else {
            sprintf(tmpchar, "%s", son_p->name);
            tmpchar[strlen(tmpchar)] = ' ';
            tmpchar[FOLDER_NAME_LEN] = '\0';
        }
        menu_show_string(FONT_W*2, SHOW_STAR_Y+FONT_H*i, tmpchar);
        son_p = son_p->next_brother;
    }

    for(int j = 0; j < FOLDER_NAME_LEN+1; j ++) {
        tmpchar[j] = ' ';
    }
    tmpchar[FOLDER_NAME_LEN] = '\0';

    for(int i = key_menu_p->father->sons_Count+1; i <= EVERY_FOLDER_NUMBER; i++)
    {
        menu_show_string(FONT_W*2, SHOW_STAR_Y+FONT_H*i, tmpchar);
    }
}

//显示箭头
static void Mymenu_show_key(void)
{
    for(int i = 1; i <= key_menu_p->father->sons_Count; i++)
    {
        if(key_menu_p->No != i)
            menu_show_string(0, SHOW_STAR_Y+FONT_H*i, "  ");
        else
            menu_show_string(0, SHOW_STAR_Y+FONT_H*i, "->");
    }
    for(int i = key_menu_p->father->sons_Count+1; i <= EVERY_FOLDER_NUMBER; i++)
    {
        menu_show_string(0, SHOW_STAR_Y+FONT_H*i, "  ");
    }
}

//显示数据
static void Mymenu_show_data(void)
{
    char clean[FOLDER_NUMBER_LEN+3];
    Folder_Menu *son_p = key_menu_p->father->son_first;
    for(int i = 1; i <= key_menu_p->father->sons_Count; i++)
    {
        switch(son_p->kind)
        {
            case bool_Box:
                for(int i = 0; i <= FOLDER_NUMBER_LEN; i++) {
                    clean[i] = ' ';
                }
                if(*(bool *)son_p->private_data == 1) {
                    clean[FOLDER_NUMBER_LEN+1] = 'Y';
                    clean[FOLDER_NUMBER_LEN+2] = '\0';
                    menu_show_string(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 2), SHOW_STAR_Y+FONT_H*i, clean);
                }
                else {
                    clean[FOLDER_NUMBER_LEN+1] = 'N';
                    clean[FOLDER_NUMBER_LEN+2] = '\0';
                    menu_show_string(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 2), SHOW_STAR_Y+FONT_H*i, clean);
                }
                break;
            case float_Box:
                if(son_p->number_box_select == 0) {
                    menu_show_char(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 2), SHOW_STAR_Y+FONT_H*i, ' ');
                    if(*(float *)(son_p->private_data) > 10)
                        menu_show_float(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 1), SHOW_STAR_Y+FONT_H*i, *(float *)(son_p->private_data), FOLDER_NUMBER_LEN-3, 2);
                    else
                        menu_show_float(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 1), SHOW_STAR_Y+FONT_H*i, *(float *)(son_p->private_data), FOLDER_NUMBER_LEN-4, 3);
                    menu_show_char(FONT_W * (COLS_SUM_LEN - 1), SHOW_STAR_Y+FONT_H*i, ' ');
                }
                if(son_p->number_box_select == 1) {
                    menu_show_char(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 2), SHOW_STAR_Y+FONT_H*i, '<');
                    if(*(float *)(son_p->private_data) > 10)
                        menu_show_float(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 1), SHOW_STAR_Y+FONT_H*i, *(float *)(son_p->private_data), FOLDER_NUMBER_LEN-3, 2);
                    else
                        menu_show_float(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 1), SHOW_STAR_Y+FONT_H*i, *(float *)(son_p->private_data), FOLDER_NUMBER_LEN-4, 3);
                    menu_show_char(FONT_W * (COLS_SUM_LEN - 1), SHOW_STAR_Y+FONT_H*i, '>');
                }
                break;
            default:
                if(son_p->kind == Normal_Folder)
                {
                    for(int j = 0; j <= FOLDER_NUMBER_LEN+2; j ++) {
                        clean[j] = ' ';
                    }
                    clean[FOLDER_NUMBER_LEN+2] = '\0';
                    menu_show_string(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 2), SHOW_STAR_Y+FONT_H*i, clean);
                }
                else
                {
                    if(son_p->number_box_select == 0)
                        menu_show_char(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 2), SHOW_STAR_Y+FONT_H*i, ' ');
                    else
                        menu_show_char(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 2), SHOW_STAR_Y+FONT_H*i, '<');
                    switch (son_p->kind)
                    {
                    case int_Box:    
                        menu_show_int(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 1), SHOW_STAR_Y+FONT_H*i, *(int *)(son_p->private_data), FOLDER_NUMBER_LEN);
                        break;
                    case uint8_Box:
                        menu_show_int(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 1), SHOW_STAR_Y+FONT_H*i, *(uint8_t *)(son_p->private_data), FOLDER_NUMBER_LEN);
                        break;
                    case int8_Box:
                        menu_show_int(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 1), SHOW_STAR_Y+FONT_H*i, *(int8_t *)(son_p->private_data), FOLDER_NUMBER_LEN);
                        break;
                    case uint16_Box:
                        menu_show_int(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 1), SHOW_STAR_Y+FONT_H*i, *(uint16_t *)(son_p->private_data), FOLDER_NUMBER_LEN);
                        break;
                    case int16_Box:
                        menu_show_int(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 1), SHOW_STAR_Y+FONT_H*i, *(int16_t *)(son_p->private_data), FOLDER_NUMBER_LEN);
                        break;
                    case uint32_Box:
                        menu_show_int(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 1), SHOW_STAR_Y+FONT_H*i, *(uint32_t *)(son_p->private_data), FOLDER_NUMBER_LEN);
                        break;
                    case int32_Box:
                        menu_show_int(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 1), SHOW_STAR_Y+FONT_H*i, *(int32_t *)(son_p->private_data), FOLDER_NUMBER_LEN);
                        break;
                    default:
                        break;
                    }
                    if(son_p->number_box_select == 0)
                        menu_show_char(FONT_W * (COLS_SUM_LEN - 1), SHOW_STAR_Y+FONT_H*i, ' ');
                    else
                        menu_show_char(FONT_W * (COLS_SUM_LEN - 1), SHOW_STAR_Y+FONT_H*i, '>');
                }
                break;
        }
        son_p = son_p->next_brother;
    }

    for(int j = 0; j <= FOLDER_NUMBER_LEN+2; j ++) {
        clean[j] = ' ';
    }
    clean[FOLDER_NUMBER_LEN+2] = '\0';
    for(int i = key_menu_p->father->sons_Count+1; i <= EVERY_FOLDER_NUMBER; i++)
    {
        menu_show_string(FONT_W * (COLS_SUM_LEN - FOLDER_NUMBER_LEN - 2), SHOW_STAR_Y+FONT_H*i, clean);
    }
}
//三、数据操作逻辑
//函数：menu_function_up / down menu_function_enter menu_function_quit
//menu_function_toggle menu_function_select menu_function_unselect
//MenuNumber_SetupCtrl_Plus / Sub
//menu_funtion_NumberPlus / NumberSub
//依次调用
void menu_show_All(void)
{
    Mymenu_show_title();
    Mymenu_show_Setup();
    Mymenu_show_task();
    Mymenu_show_key();
    Mymenu_show_data();
}

//上
void menu_function_up(void)
{
    //上一项
    key_menu_p = key_menu_p->last_brother;
}

//下
void menu_function_down(void)
{
    //下一项
    key_menu_p = key_menu_p->next_brother;
}

//确定
void menu_function_enter(void)
{
    if(key_menu_p->son_first == NULL) return ;
    //进入
    key_menu_p = key_menu_p->son_first;
}

//退出
void menu_function_quit(void)
{
    if(key_menu_p->father->father == NULL) return ;
    //退出
    key_menu_p = key_menu_p->father;
}

//反转check_box变量
void menu_function_toggle(void)
{
    //反转
    *(bool *)(key_menu_p->private_data) = !*(bool *)(key_menu_p->private_data);
}

//选中调整变量
void menu_function_select(void)
{
    //使能被选中项
    key_menu_p->number_box_select = 1;
}

//取消选中变量
void menu_function_unselect(void)
{
    //使能被选中项
    key_menu_p->number_box_select = 0;
}

//步进参数调整
void MenuNumber_SetupCtrl_Plus(void)
{
    SetupIndex = (SetupIndex + 1) % SETUP_LEN;
}

//步进参数调整
void MenuNumber_SetupCtrl_Sub(void)
{
    SetupIndex = (SetupIndex - 1 + SETUP_LEN) % SETUP_LEN;
}


void menu_funtion_NumberPlus(void)
{
    switch(key_menu_p->kind)
    {
        case int_Box:
        {
            if(!key_menu_p -> isLimit)
                *(int *)(key_menu_p->private_data) = *(int *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            else
                *(int *)(key_menu_p->private_data) = ( *(int *)(key_menu_p->private_data) + SetupNumber[SetupIndex] > key_menu_p->limit_max ) ? key_menu_p->limit_max : *(int *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            break;
        }
        case float_Box:
        {
            if(!key_menu_p -> isLimit)
                *(float *)(key_menu_p->private_data) = *(float *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            else
                *(float *)(key_menu_p->private_data) = ( *(float *)(key_menu_p->private_data) + SetupNumber[SetupIndex] >  key_menu_p->limit_max ) ? key_menu_p->limit_max : *(float *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            break;
        }
        case uint8_Box:
        {
            if(!key_menu_p -> isLimit)
                *(uint8_t *)(key_menu_p->private_data) = *(uint8_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            else
                *(uint8_t *)(key_menu_p->private_data) = ( *(uint8_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex] > key_menu_p->limit_max ) ? key_menu_p->limit_max : *(uint8_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            break;
        }
        case int8_Box:
        {
            if(!key_menu_p -> isLimit)
                *(int8_t *)(key_menu_p->private_data) = *(int8_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            else
                *(int8_t *)(key_menu_p->private_data) = ( *(int8_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex] > key_menu_p->limit_max ) ? key_menu_p->limit_max : *(int8_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            break;
        }
        case uint16_Box:
        {
            if(!key_menu_p -> isLimit)
                *(uint16_t *)(key_menu_p->private_data) = *(uint16_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            else
                *(uint16_t *)(key_menu_p->private_data) = ( *(uint16_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex] > key_menu_p->limit_max ) ? key_menu_p->limit_max : *(uint16_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            break;
        }
        case int16_Box:
        {
            if(!key_menu_p -> isLimit)
                *(int16_t *)(key_menu_p->private_data) = *(int16_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            else
                *(int16_t *)(key_menu_p->private_data) = ( *(int16_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex] > key_menu_p->limit_max ) ? key_menu_p->limit_max : *(int16_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            break;
        }
        case uint32_Box:
        {
            if(!key_menu_p -> isLimit)
                *(uint32_t *)(key_menu_p->private_data) = *(uint32_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            else
                *(uint32_t *)(key_menu_p->private_data) = ( *(uint32_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex] > key_menu_p->limit_max ) ? key_menu_p->limit_max : *(uint32_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            break;
        }
        case int32_Box:
        {
            if(!key_menu_p -> isLimit)
                *(int32_t *)(key_menu_p->private_data) = *(int32_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            else
                *(int32_t *)(key_menu_p->private_data) = ( *(int32_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex] > key_menu_p->limit_max ) ? key_menu_p->limit_max : *(int32_t *)(key_menu_p->private_data) + SetupNumber[SetupIndex];
            break;
        }
        default:
        break;
    }
}

void menu_funtion_NumberSub(void)
{
    switch(key_menu_p->kind)
    {
        case int_Box:
        {
            if(!key_menu_p -> isLimit)
                *(int *)(key_menu_p->private_data) = *(int *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            else
                *(int *)(key_menu_p->private_data) = ( *(int *)(key_menu_p->private_data) - SetupNumber[SetupIndex] < key_menu_p->limit_min) ? key_menu_p->limit_min : *(int *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            break;
        }
        case float_Box:
        {
            if(!key_menu_p -> isLimit)
                *(float *)(key_menu_p->private_data) = *(float *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            else
                *(float *)(key_menu_p->private_data) = ( *(float *)(key_menu_p->private_data) - SetupNumber[SetupIndex] < key_menu_p->limit_min) ? key_menu_p->limit_min : *(float *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            break;
        }
        case uint8_Box:
        {
            if(!key_menu_p -> isLimit)
                *(uint8_t *)(key_menu_p->private_data) = *(uint8_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            else
                *(uint8_t *)(key_menu_p->private_data) = ( *(uint8_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex] < key_menu_p->limit_min) ? key_menu_p->limit_min : *(uint8_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            break;
        }
        case int8_Box:
        {
            if(!key_menu_p -> isLimit)
                *(int8_t *)(key_menu_p->private_data) = *(int8_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            else
                *(int8_t *)(key_menu_p->private_data) = ( *(int8_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex] < key_menu_p->limit_min) ? key_menu_p->limit_min : *(int8_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            break;
        }
        case uint16_Box:
        {
            if(!key_menu_p -> isLimit)
                *(uint16_t *)(key_menu_p->private_data) = *(uint16_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            else
                *(uint16_t *)(key_menu_p->private_data) = ( *(uint16_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex] < key_menu_p->limit_min) ? key_menu_p->limit_min : *(uint16_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            break;
        }
        case int16_Box:
        {
            if(!key_menu_p -> isLimit)
                *(int16_t *)(key_menu_p->private_data) = *(int16_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            else
                *(int16_t *)(key_menu_p->private_data) = ( *(int16_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex] < key_menu_p->limit_min) ? key_menu_p->limit_min : *(int16_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            break;
        }
        case uint32_Box:
        {
            if(!key_menu_p -> isLimit)
                *(uint32_t *)(key_menu_p->private_data) = *(uint32_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            else
                *(uint32_t *)(key_menu_p->private_data) = ( *(uint32_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex] < key_menu_p->limit_min) ? key_menu_p->limit_min : *(uint32_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            break;
        }
        case int32_Box:
        {
            if(!key_menu_p -> isLimit)
                *(int32_t *)(key_menu_p->private_data) = *(int32_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            else
                *(int32_t *)(key_menu_p->private_data) = ( *(int32_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex] < key_menu_p->limit_min) ? key_menu_p->limit_min : *(int32_t *)(key_menu_p->private_data) - SetupNumber[SetupIndex];
            break;
        }
        default:
        break;
    }
}
//四、按键映射，四按键或五按键模式
#ifdef FOUR_BUTTON
//方向上--控制逻辑
void Menu_upFuntion(void)
{
    switch (key_menu_p->kind)
    {
    case Normal_Folder:
        menu_function_up();
        break;
    
    case bool_Box:
        menu_function_up();
        break;

    default:
        //如果被选中 则调整参数
        if(key_menu_p->number_box_select == 1)
            menu_funtion_NumberPlus();
        else
            menu_function_up();
        break;
    }
}

//方向下--控制逻辑
void Menu_downFuntion(void)
{
    switch (key_menu_p->kind)
    {
    case Normal_Folder:
        menu_function_down();
        break;
    
    case bool_Box:
        menu_function_down();
        break;

    default:
        //如果被选中 则调整参数
        if(key_menu_p->number_box_select == 1)
            menu_funtion_NumberSub();
        else
            menu_function_down();
        break;
    }
}

//确定--控制逻辑
void Menu_enterFuntion(void)
{
    switch (key_menu_p->kind)
    {
    case Normal_Folder:
        menu_function_enter();
        break;
    
    case bool_Box:
        menu_function_toggle();
        break;

    default:
        if(key_menu_p->number_box_select == 1)
            menu_function_unselect();
        else
            menu_function_select();
        break;
    }
}

//取消--控制逻辑
void Menu_quitFuntion(void)
{
    switch (key_menu_p->kind)
    {
    case Normal_Folder:
            menu_function_quit();
        break;
    
    case bool_Box:
            menu_function_quit();
        break;

    default:
        if(key_menu_p->number_box_select == 1)
            MenuNumber_SetupCtrl_Plus();
        else
            menu_function_quit();
        break;
    }
}
#endif

#ifdef FIVE_BUTTON
void Menu_upButton(void)
{
    switch (key_menu_p->kind)
    {
    case Normal_Folder:
        menu_function_up();
        break;
    
    case bool_Box:
        menu_function_up();
        break;

    default:
        //如果被选中 则调整参数
        if(key_menu_p->number_box_select == 1)
            menu_funtion_NumberPlus();
        else
            menu_function_up();
        break;
    }
}

void Menu_downButton(void)
{
    switch (key_menu_p->kind)
    {
    case Normal_Folder:
        menu_function_down();
        break;
    
    case bool_Box:
        menu_function_down();
        break;

    default:
        //如果被选中 则调整参数
        if(key_menu_p->number_box_select == 1)
            menu_funtion_NumberSub();
        else
            menu_function_down();
        break;
    }
}

void Menu_leftButton(void)
{
    switch (key_menu_p->kind)
    {
    case Normal_Folder:
            menu_function_quit();
        break;
    
    case bool_Box:
            menu_function_quit();
        break;

    default:
        if(key_menu_p->number_box_select == 1)
            MenuNumber_SetupCtrl_Sub();
        else {
            menu_function_quit();
        }
        break;
    }
}

void Menu_rightButton(void)
{
    switch (key_menu_p->kind)
    {
    case Normal_Folder:
        menu_function_enter();
        break;
    
    case bool_Box:
        menu_function_toggle();
        break;

    default:
        if(key_menu_p->number_box_select == 1)
            MenuNumber_SetupCtrl_Plus();
        else
            menu_function_select();
        break;
    }
}

void Menu_midButton(void)
{
    switch (key_menu_p->kind)
    {
    case Normal_Folder:
        menu_function_enter();
        break;
    
    case bool_Box:
        menu_function_toggle();
        break;

    default:
        if(key_menu_p->number_box_select == 1)
            menu_function_unselect();
        else
            menu_function_select();
        break;
    }
}

#endif
