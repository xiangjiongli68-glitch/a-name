/*******************************************************************************************************************************
 *  代码用途: 多级菜单
*************************/
#ifndef __COMMON_MENU_H_
#define __COMMON_MENU_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

//文件类型
typedef enum {
    Normal_Folder,                          //文件夹
    bool_Box,                               //布尔型
    int_Box,                                //int型
    float_Box,                              //float型
    uint8_Box,                              //uint8型
    int8_Box,                               //int8型
    uint16_Box,                             //uint16型
    int16_Box,                              //int16型
    uint32_Box,                             //uint32型
    int32_Box,                              //int32型
}Folder_Class;

typedef struct Folder_Menu {
    const char *name;                       //文件名称
    uint8_t No;                             //当前文件夹下的位次
    Folder_Class kind;                      //文件种类

    uint8_t sons_Count;                     //子文件个数

    struct Folder_Menu *father;             //父文件节点
    struct Folder_Menu *son_first;          //第一个子文件
    struct Folder_Menu *next_brother;       //下一个兄弟文件
    struct Folder_Menu *last_brother;       //上一个兄弟文件
    
    void*   private_data;                   //私有数据
    uint8_t number_box_select;              //数值项是否被选中

    /* 3.3更新限幅 */
    bool isLimit;
    float limit_min;
    float limit_max;

} Folder_Menu;

extern Folder_Menu myMenu;
extern Folder_Menu *key_menu_p;

//----------------------------------------------------以上内容可以无视---------------------------------------------------------------------

#define MAX_MENU_ITEMS          (128)           //最大分配内存(提供的菜单数量)

////创建文件夹函数
//void create_Menu_Folder(Folder_Menu *father, Folder_Menu *me, const char *name);
//void create_Menu_BoolBox(Folder_Menu *father, Folder_Menu *me, const char *name, void *check);
//void create_Menu_NumberBox(Folder_Menu *father, Folder_Menu *me, const char *name, void *number, Folder_Class kind);
///* 3.3版本更新创建limit变量 */
//void create_Menu_LimitNumberBox(Folder_Menu *father, Folder_Menu *me, const char *name, void *number, Folder_Class kind, float limit_Min, float limit_Max);

/*创建文件夹功能 指针函数*/
Folder_Menu* dynamicCreate_Menu_Folder(Folder_Menu *father, const char *name);
/*文件夹中创建文件   不带限幅  带限幅*/
Folder_Menu* dynamicCreate_Menu_NumberBox(Folder_Menu *father, const char *name, void *number, Folder_Class kind);
Folder_Menu* dynamicCreate_Menu_LimitNumberBox(Folder_Menu *father, const char *name, void *number, Folder_Class kind, float limit_Min, float limit_Max);


/* 菜单生成函数（内部调用无需理会） */
void All_Folder_Menu_Init(Folder_Menu *Menu);


#endif
