/*******************************************************************************************************************************
 *  代码用途: 多级菜单
*************************/
#include "common_menu.h"

//用于动态分配菜单的--------静态内存池(用于分配动态创建的菜单)-------

static Folder_Menu menu_pool[MAX_MENU_ITEMS];   //内存池
static uint16_t pool_index = 0;                 //内存池索引

//用于动态分配菜单的--------静态内存池(用于分配动态创建的菜单)-------



//-------------------------------------------------------------------------------------------------------------------
// 函数简介     内部通用创建函数
// 参数说明     father      父节点
// 参数说明     me          文件夹
// 参数说明     name        文件夹名称
// 参数说明     kind        文件类型
// 参数说明     data        绑定数据
// 参数说明     isLimit     是否限幅
// 参数说明     limit_min   最小限幅
// 参数说明     limit_max   最大限幅
// 返回参数     void
// 使用示例     create_Menu_Folder(&myMenu, &folder1, "folder1");
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
static void __create_menu_item(Folder_Menu *father, Folder_Menu *me, 
                               const char *name, Folder_Class kind, 
                               void *data, bool isLimit, 
                               float limit_min, float limit_max)
{
    if(father->kind != Normal_Folder) return;

    // 通用属性初始化
    me->name = name;
    me->kind = kind;
    me->private_data = data;
    me->isLimit = isLimit;
    me->limit_min = limit_min;
    me->limit_max = limit_max;
    me->number_box_select = 0;
    me->sons_Count = 0;
    
    // 链表初始化
    me->father = father;
    me->next_brother = NULL;
    me->last_brother = NULL;
    me->son_first = NULL;

    // 添加到父节点链表
    if(!father->son_first) {
        father->son_first = me;
        me->No = 1;
    } else {
        Folder_Menu *p = father->son_first;
        while(p->next_brother) p = p->next_brother;
        p->next_brother = me;
        me->last_brother = p;
        me->No = father->sons_Count + 1;
    }
    father->sons_Count++;
}


//66-139 手动建立目录
// 宏定义简化创建函数
#define CREATE_MENU_ITEM(father, me, name, kind, data) \
    __create_menu_item(father, me, name, kind, data, false, 0, 0)

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向父节点注册文件夹
// 参数说明     father      父节点
// 参数说明     me          文件夹
// 参数说明     name        文件夹名称
// 返回参数     void
// 使用示例     create_Menu_Folder(&myMenu, &folder1, "folder1");
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void create_Menu_Folder(Folder_Menu *father, Folder_Menu *me, const char *name)
{
    CREATE_MENU_ITEM(father, me, name, Normal_Folder, NULL);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向父节点注册BOOLBOX
// 参数说明     father      父节点
// 参数说明     me          复选框
// 参数说明     name        复选框名称
// 参数说明     bool        复选框指向的变量
// 返回参数     void
// 使用示例     create_Menu_BoolBox(&myMenu, &box, "box1", &box1);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void create_Menu_BoolBox(Folder_Menu *father, Folder_Menu *me, const char *name, void *check)
{
    CREATE_MENU_ITEM(father, me, name, bool_Box, check);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向父节点注册任意数值型
// 参数说明     father          父节点
// 参数说明     me              数值框
// 参数说明     name            数值框名称
// 参数说明     uint8_number    数值框指向的变量
// 返回参数     void
// 使用示例     create_Menu_NumberBox(&myMenu, &numbox1, "numbox1", &num1, uint8_Box);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void create_Menu_NumberBox(Folder_Menu *father, Folder_Menu *me, const char *name, void *number, Folder_Class kind)
{
    CREATE_MENU_ITEM(father, me, name, kind, number);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向父节点注册带限幅数值型
// 参数说明     father          父节点
// 参数说明     me              数值框
// 参数说明     name            数值框名称
// 参数说明     uint8_number    数值框指向的变量
// 返回参数     void
// 使用示例     create_Menu_LimitNumberBox(&myMenu, &numbox1, "numbox1", &num1, uint8_Box, 0, 180);
// 备注信息
//-------------------------------------------------------------------------------------------------------------------
void create_Menu_LimitNumberBox(Folder_Menu *father, Folder_Menu *me, const char *name, void *number, Folder_Class kind, float limit_Min, float limit_Max)
{
    __create_menu_item(father, me, name, kind, number, true, limit_Min, limit_Max);
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向父节点动态注册文件夹
// 参数说明     father          父节点
// 参数说明     name            文件夹名称
// 返回参数     Folder_Menu*    结构体指针
// 使用示例     Folder_Menu* folder = dynamicCreate_Menu_Folder(&myMenu, "folder1");
// 备注信息     文件夹创建一定要使用结构体指针去接，否则无法向其添加子节点
//-------------------------------------------------------------------------------------------------------------------





//145-188自动建立目录/动态注册
Folder_Menu* dynamicCreate_Menu_Folder(Folder_Menu *father, const char *name)
{
    if (pool_index >= MAX_MENU_ITEMS) return NULL;
    Folder_Menu *me = &menu_pool[pool_index++];

    create_Menu_Folder(father, me, name);
    return me;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向父节点动态注册任意数值型
// 参数说明     father          父节点
// 参数说明     name            数值框名称
// 参数说明     uint8_number    数值框指向的变量
// 返回参数     Folder_Menu*    结构体指针
// 使用示例     Folder_Menu* test = dynamicCreate_Menu_NumberBox(&myMenu, "numbox1", &num1, uint8_Box);
// 备注信息     可以不使用变量承接指针
//-------------------------------------------------------------------------------------------------------------------
Folder_Menu* dynamicCreate_Menu_NumberBox(Folder_Menu *father, const char *name, void *number, Folder_Class kind)
{
    if (pool_index >= MAX_MENU_ITEMS) return NULL;
    Folder_Menu *me = &menu_pool[pool_index++];

    create_Menu_NumberBox(father, me, name, number, kind);
    return me;
}

//-------------------------------------------------------------------------------------------------------------------
// 函数简介     向父节点动态注册带限幅数值型
// 参数说明     father          父节点
// 参数说明     name            数值框名称
// 参数说明     uint8_number    数值框指向的变量
// 返回参数     Folder_Menu*    结构体指针
// 使用示例     Folder_Menu* test = dynamicCreate_Menu_LimitNumberBox(&myMenu, "numbox1", &num1, uint8_Box, 0, 180);
// 备注信息     可以不使用变量承接指针
//-------------------------------------------------------------------------------------------------------------------
Folder_Menu* dynamicCreate_Menu_LimitNumberBox(Folder_Menu *father, const char *name, void *number, Folder_Class kind, float limit_Min, float limit_Max)
{
    if (pool_index >= MAX_MENU_ITEMS) return NULL;
    Folder_Menu *me = &menu_pool[pool_index++];

    create_Menu_LimitNumberBox(father, me, name, number, kind, limit_Min, limit_Max);
    return me;
}






/*----------------------
/   所有子项目初始化
----------------------*/
void All_Folder_Menu_Init(Folder_Menu *Menu)
{
    if(Menu->son_first == NULL) {
        return ;
    }
    Folder_Menu *hp = Menu->son_first;
    Folder_Menu *p = Menu->son_first;

    if(hp->next_brother == NULL) {
        All_Folder_Menu_Init(p);
    }
    while(p->next_brother != NULL) {
        if(p->kind == Normal_Folder) {
            All_Folder_Menu_Init(p);
        }
        p = p->next_brother;
    }
    if(hp->next_brother != NULL) {
        All_Folder_Menu_Init(p);
    }

    p->next_brother = hp;
    hp->last_brother = p;
}
