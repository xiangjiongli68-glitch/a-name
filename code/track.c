//巡线函数逻辑，也就是对巡线参数进行处理
#include "track.h"

int32_t search_way = 1;

uint8 l_border[Image_Y];
uint8 r_border[Image_Y];
uint8 mid_line[Image_Y];

uint8 l_found;
uint8 r_found;

int16 width[Image_Y] = 
{
    2, 3, 3, 4, 4, 4, 4, 4, 4, 
    4, 4, 4, 4, 4, 4, 4, 4, 4, 
    4, 4, 4, 4, 4, 5, 5, 5, 6, 
    5, 5, 5, 5, 5, 5, 6, 6, 6, 
    6, 6, 6, 6, 6, 6, 6, 7, 7, 
    7, 7, 7, 7, 7, 8, 8, 8, 8, 
};

/**
 * 计算整数的绝对值
 *
 * @param value 输入的整数值
 * @return 返回value的绝对值，如果value为非负数则返回原值，否则返回其相反数
 */
int my_abs(int value)
{
    if (value >= 0) return value;
    else return -value;
}


/**
 * 计算浮点数的绝对值
 *
 * @param value 输入的浮点数值
 * @return 返回value的绝对值，如果value为正数或零则返回原值，如果value为负数则返回其相反数
 */
float my_abs_f(float value)
{
    if (value >= 0) return value;
    else return -value;
}

/**
 * 返回两个整数中的较大值
 *
 * @param x 第一个整数
 * @param y 第二个整数
 * @return 返回x和y中的较大值
 */
int my_max(int x, int y)
{
    return (x > y) ? x : y;
}

/**
 * @brief 在二值化图像中搜索车道线边界并计算中线位置
 *
 * 该函数根据不同的搜索策略（search_way）从图像底部开始逐行向上搜索车道线的左右边界，
 * 通过检测黑色-白色-白色的像素模式来定位边界，并计算每行的中线位置。
 *
 * 搜索策略：
 * - search_way == 1: 自适应起始位置搜索，支持单边边界情况
 * - search_way == 2: 从图像宽度1/6处开始搜索
 * - search_way == 3: 从图像宽度1/4处开始搜索
 * - search_way == 4: 从图像宽度1/3处开始搜索
 *
 * 边界检测模式：寻找 Black-White-White 的像素序列
 *
 * @return void 无返回值，结果存储在全局数组 l_border、r_border、mid_line 中
 */
//巡线核心函数，一直到350行左右
void search_line(void)
{
    uint8 i, j;
    uint8 last_l_border = Image_X / 2;

    if(search_way == 1)
    {
        // 从图像底部开始逐行向上搜索
        for (i = Image_Y - 1; i > 0; i--)
        {
            l_found = 0;
            r_found = 0;
            
            uint8 l_start = 0;

            if (i < Image_Y - 1)
            {
                // 根据上一行左边界位置确定当前行搜索起始点，限制搜索范围
                l_start = (last_l_border > Image_X / 8) ? last_l_border - Image_X / 8 : 0;

                if (l_start > Image_X - 1)
                    l_start = Image_X - 1;
            }

            // 搜索左边界：寻找 Black-White-White 模式
            for (j = l_start; j + 2 < Image_X - 1; j++)
            {
                if (bin_image[i][j] == Black && bin_image[i][j + 1] == White && bin_image[i][j + 2] == White)
                {
                    l_found = 1;
                    l_border[i] = j;
                    last_l_border = j;
                    break;
                }
            }

            if (l_found)
            {
                // 基于找到的左边界确定右边界搜索起始点
                uint8 r_start = l_border[i] + Image_X / 6;

                if (r_start >= Image_X - 1)
                    r_start = Image_X - 1;

                // 从右向左搜索右边界：寻找 Black-White-White 模式
                for (uint8 k = r_start; k > l_border[i]; k--)
                {
                    if (bin_image[i][k] == Black && bin_image[i][k - 1] == White && bin_image[i][k - 2] == White)
                    {
                        r_found = 1;
                        r_border[i] = k;
                        break;
                    }
                }
            }

            // 根据左右边界检测结果计算中线位置
            if (l_found && r_found)
            {
                // 双边界都找到：中线为左右边界中点
                mid_line[i] = (l_border[i] + r_border[i]) / 2;
            }
            else if (l_found)
            {
                // 只找到左边界：基于预设宽度计算中线
                mid_line[i] = l_border[i] + (width[i] / 2);
            }
            else if (r_found)
            {
                // 只找到右边界：基于预设宽度计算中线
                mid_line[i] = r_border[i] - (width[i] / 2);
            }
            else
            {
                // 都未找到：使用上一行中线位置或图像中心
                mid_line[i] = (i == Image_Y - 1 ? Image_X / 2 : mid_line[i + 1]);
            }
        }
    }
    else if(search_way == 2)
    {
        // 从图像底部开始逐行向上搜索
        for (i = Image_Y - 1; i > 0; i--)
        {
            l_found = 0;
            r_found = 0;
            
            uint8 l_start = 0;

            if (i < Image_Y - 1)
            {
                // 根据上一行左边界位置确定当前行搜索起始点，限制搜索范围
                l_start = (last_l_border > Image_X / 8) ? last_l_border - Image_X / 8 : 0;
                
                if (l_start > Image_X - 1)
                    l_start = Image_X - 1;
            }

            // 从图像宽度1/6处开始搜索左边界：寻找 Black-White-White 模式
            for (j = Image_X / 6; j + 2 < Image_X - 1; j++)
            {
                if (bin_image[i][j] == Black && bin_image[i][j + 1] == White && bin_image[i][j + 2] == White)
                {
                    l_found = 1;
                    l_border[i] = j;
                    last_l_border = j;
                    break;
                }
            }

            if (l_found)
            {
                // 基于找到的左边界确定右边界搜索起始点
                uint8 r_start = l_border[i] + Image_X / 6;

                if (r_start >= Image_X - 1)
                    r_start = Image_X - 1;

                // 从右向左搜索右边界：寻找 Black-White-White 模式
                for (uint8 k = r_start; k > l_border[i]; k--)
                {
                    if (bin_image[i][k] == Black && bin_image[i][k - 1] == White && bin_image[i][k - 2] == White)
                    {
                        r_found = 1;
                        r_border[i] = k;
                        break;
                    }
                }
            }

            // 根据左右边界检测结果计算中线位置
            if (l_found && r_found)
            {
                // 双边界都找到：中线为左右边界中点
                mid_line[i] = (l_border[i] + r_border[i]) / 2;
            }
            else
            {
                // 都未找到：使用上一行中线位置或图像中心
                mid_line[i] = (i == Image_Y - 1 ? Image_X / 2 : mid_line[i + 1]);
            }
        }
    }
    else if(search_way == 3)
    {
        // 从图像底部开始逐行向上搜索
        for (i = Image_Y - 1; i > 0; i--)
        {
            l_found = 0;
            r_found = 0;
            
            uint8 l_start = 0;

            if (i < Image_Y - 1)
            {
                // 根据上一行左边界位置确定当前行搜索起始点，限制搜索范围
                l_start = (last_l_border > Image_X / 8) ? last_l_border - Image_X / 8 : 0;
                
                if (l_start > Image_X - 1)
                    l_start = Image_X - 1;
            }

            // 从图像宽度1/4处开始搜索左边界：寻找 Black-White-White 模式
            for (j = Image_X / 4; j + 2 < Image_X - 1; j++)
            {
                if (bin_image[i][j] == Black && bin_image[i][j + 1] == White && bin_image[i][j + 2] == White)
                {
                    l_found = 1;
                    l_border[i] = j;
                    last_l_border = j;
                    break;
                }
            }

            if (l_found)
            {
                // 基于找到的左边界确定右边界搜索起始点
                uint8 r_start = l_border[i] + Image_X / 6;

                if (r_start >= Image_X - 1)
                    r_start = Image_X - 1;

                // 从右向左搜索右边界：寻找 Black-White-White 模式
                for (uint8 k = r_start; k > l_border[i]; k--)
                {
                    if (bin_image[i][k] == Black && bin_image[i][k - 1] == White && bin_image[i][k - 2] == White)
                    {
                        r_found = 1;
                        r_border[i] = k;
                        break;
                    }
                }
            }

            // 根据左右边界检测结果计算中线位置
            if (l_found && r_found)
            {
                // 双边界都找到：中线为左右边界中点
                mid_line[i] = (l_border[i] + r_border[i]) / 2;
            }
            else
            {
                // 都未找到：使用上一行中线位置或图像中心
                mid_line[i] = (i == Image_Y - 1 ? Image_X / 2 : mid_line[i + 1]);
            }
        }
    }
    else if(search_way == 4)
    {
        // 从图像底部开始逐行向上搜索
        for (i = Image_Y - 1; i > 0; i--)
        {
            l_found = 0;
            r_found = 0;
            
            uint8 l_start = 0;

            if (i < Image_Y - 1)
            {
                // 根据上一行左边界位置确定当前行搜索起始点，限制搜索范围
                l_start = (last_l_border > Image_X / 8) ? last_l_border - Image_X / 8 : 0;
                
                if (l_start > Image_X - 1)
                    l_start = Image_X - 1;
            }

            // 从图像宽度1/3处开始搜索左边界：寻找 Black-White-White 模式
            for (j = Image_X / 3; j + 2 < Image_X - 1; j++)
            {
                if (bin_image[i][j] == Black && bin_image[i][j + 1] == White && bin_image[i][j + 2] == White)
                {
                    l_found = 1;
                    l_border[i] = j;
                    last_l_border = j;
                    break;
                }
            }

            if (l_found)
            {
                // 基于找到的左边界确定右边界搜索起始点
                uint8 r_start = l_border[i] + Image_X / 6;

                if (r_start >= Image_X - 1)
                    r_start = Image_X - 1;

                // 从右向左搜索右边界：寻找 Black-White-White 模式
                for (uint8 k = r_start; k > l_border[i]; k--)
                {
                    if (bin_image[i][k] == Black && bin_image[i][k - 1] == White && bin_image[i][k - 2] == White)
                    {
                        r_found = 1;
                        r_border[i] = k;
                        break;
                    }
                }
            }

            // 根据左右边界检测结果计算中线位置
            if (l_found && r_found)
            {
                // 双边界都找到：中线为左右边界中点
                mid_line[i] = (l_border[i] + r_border[i]) / 2;
            }
            else
            { 
                // 都未找到：使用上一行中线位置或图像中心
                mid_line[i] = (i == Image_Y - 1 ? Image_X / 2 : mid_line[i + 1]);
            }
        }
    }
}

uint8 l_edge, r_edge;
uint8 l_edge_up, r_edge_up, l_edge_down, r_edge_down;

/**
 * @brief 边缘检测函数，用于检测图像中的左右边缘
 *
 * 该函数通过扫描二值化图像来找到左右两侧的边缘位置，
 * 从图像底部向上扫描，记录边缘的上下边界位置
 *
 * @param void 无参数
 * @return void 无返回值
 */
//边缘检测函数，用于定位赛道最中心，一直到430行左右
void edge_detect(void)
{
    // 初始化左右边缘检测相关变量
    l_edge = 0, r_edge = 0;
    l_edge_up = 0, l_edge_down = 0;
    r_edge_up = 0, r_edge_down = 0;

    bool l_detect_end = 0;

    // 从图像底部开始向上扫描，检测左边缘
    for (uint8 i = Image_Y - 3; i > 8; i--)
    {
        // 在当前行的左侧区域扫描白色像素
        for (uint8 j = 3; j < 8; j++)
        {
            if (bin_image[i][j] == White)
            {
                if (!l_edge)
                {
                    l_edge_down = i;
                    l_edge = 1;
                }

                // 检测到白色像素上方为黑色像素时，确定边缘上边
                if (bin_image[i - 1][j] == Black)
                {
                    l_edge_up = i;
                    l_detect_end = 1;
                    break;
                }
                break;
            }
        }
        if (l_detect_end) break;
    }

    bool r_detect_end = 0;

    // 从图像底部开始向上扫描，检测右边缘
    for (uint8 i = Image_Y - 3; i > 8; i--)
    {
        // 在当前行的右侧区域扫描白色像素
        for (uint8 j = Image_X - 3; j > Image_X - 8; j--)
        {
            if (bin_image[i][j] == White)
            {
                if (!r_edge)
                {
                    r_edge_down = i;
                    r_edge = 1;
                }

                // 检测到白色像素上方为黑色像素时，确定边缘上边界
                if (bin_image[i - 1][j] == Black)
                {
                    r_edge_up = i;
                    r_detect_end = 1;
                    break;
                }
                break;
            }
        }
        if (r_detect_end) break;
    }
}

/**
 * @brief 计算并设置图像中两点间连线的中心线及边界
 *
 * 该函数根据给定的两个点坐标(x1,y1)和(x2,y2)，计算它们之间连线在每行的交点，
 * 并将结果存储到全局数组中，用于图像处理中的中心线和边界跟踪
 *
 * @param x1 第一个点的x坐标
 * @param y1 第一个点的y坐标
 * @param x2 第二个点的x坐标
 * @param y2 第二个点的y坐标
 * @return 无返回值
 */
//画直线，将断点脑补成连续的，给定两个点，直接连
void center_change(uint8 x1, uint8 y1, uint8 x2, uint8 y2)
{
    uint8 i, a1, a2, temp;
    uint8 hx;

    // 对输入坐标进行边界检查，确保在图像范围内
    if (x1 > Image_X - 1)
        x1 = Image_X - 1;
    else if (x1 <= 0)
        x1 = 0;
    if (y1 > Image_Y - 1)
        y1 = Image_Y - 1;
    else if (y1 <= 0)
        y1 = 0;
    if (x2 > Image_X - 1)
        x2 = Image_X - 1;
    else if (x2 <= 0)
        x2 = 0;
    if (y2 > Image_Y - 1)
        y2 = Image_Y - 1;
    else if (y2 <= 0)
        y2 = 0;

    // 保存原始y坐标值
    a1 = y1;
    a2 = y2;

    // 确保a1为较小的y值，a2为较大的y值，用于后续循环遍历
    if (a1 > a2)
    {
        temp = a1;
        a1 = a2;
        a2 = temp;
    }

    // 遍历从a1到a2的每一行，计算直线在该行的x坐标交点
    for (i = a1; i <= a2; i++)
    {
        // 使用线性插值公式计算直线方程在当前行的x坐标
        hx = (i - y1) * (x2 - x1) / (y2 - y1) + x1;
        // 对计算结果进行边界检查
        if (hx >= Image_X)
            hx = Image_X - 1;
        else if (hx <= 0)
            hx = 0;
        // 将计算结果存储到对应的全局数组中
        mid_line[i] = hx;
        l_border[i] = hx;
        r_border[i] = hx;
    }
}

/**
 * 修复图像中的断线
 * 该函数通过检测图像中的断点和空白行，对断裂的线条进行修复或补全
 * 主要用于处理二值化图像中由于噪声或缺失导致的线条中断问题
 *
 * @param 无参数
 * @return 无返回值
 */
//它的功能是逻辑判断：负责找出哪里断了，并决定要不要连起来。
void repair_line(void)
{
    uint8 i, j;
    uint8 rupt_point = 0, blank_rows = 0;

    // 从图像底部向上扫描，寻找断点并进行修复
    for (i = Image_Y - 5; i > 3; i--)
    {
        // 检查是否为近距离断点，尝试连接断开的线条
        if (blank_rows && (rupt_point - i < 15) && rupt_point)
        {
            if (bin_image[i][mid_line[i]] == White && my_abs(mid_line[i] - mid_line[rupt_point]) <= 6)
            {
                if (rupt_point > Image_Y - 8)
                    center_change(Image_X / 2, Image_Y - 5, mid_line[i], i);
                else
                    center_change(mid_line[rupt_point], rupt_point, mid_line[i], i);
                rupt_point = 0;
                blank_rows = 0;
            }
        }
        // 检查是否为远距离断点，进行线条延伸修复
        else if (blank_rows && rupt_point - i >= 15 && rupt_point < 35 && rupt_point)
        {
            for (j = rupt_point; j > 5; j--)
            {
                l_border[j] = mid_line[rupt_point - 1];
                r_border[j] = mid_line[rupt_point - 1];
                mid_line[j] = mid_line[rupt_point - 1];
            }
            rupt_point = 0;
            blank_rows = 0;
            break;
        }

        // 检测到黑色像素且未标记空白行时，记录断点位置
        if ((bin_image[i][mid_line[i]] == Black) && (!blank_rows))
        {
            rupt_point = i + 1;
            blank_rows = 1;
        }
    }
}

//速度快的时候可以把权重放上面一点，前瞻会更好
uint8 image_err_weight[Image_Y] = 
{
    0, 0, 0, 0, 0, 0, 0, 0, 
    0, 0, 0, 0, 10, 10, 9, 9, 
    8, 8, 8, 8, 7, 7, 7, 7, 
    5, 5, 5, 5, 3, 3, 3, 3, 
    2, 2, 2, 2, 2, 2, 2, 2, 
    2, 2, 2, 2, 1, 1, 1, 1, 
};

float image_err = 0.0f;

/**
 * @brief 图像误差处理函数
 * @description 计算图像中线偏离中心的加权误差，并进行滤波和限幅处理
 * @param 无参数
 * @return 无返回值
 */
//将复杂的赛道中线数据，浓缩成一个简单的数值image_err；这个参数非常重要
void image_err_proc(void)
{
    float err = 0;
    float err_sum = 0;
    float weight_sum = 0;
    static float err_last;

    // 计算加权误差总和和权重总和
    for (uint8 i = 0; i < Image_Y; i++)
    {
        err_sum += ((mid_line[i]) - Image_X / 2) * image_err_weight[i];
        weight_sum += image_err_weight[i];
    }

    // 计算平均误差并进行一阶低通滤波
    err = err_sum / weight_sum;
    err = err * 0.9f + err_last * 0.1f;
    err_last = err;

    // 误差值限幅处理，限制在[-32, 32]范围内
    //>0,线偏右，左转     <0,线偏左，右转
    if (err >= 32)
        err = 32;
    if (err <= -32)
        err = -32;
    image_err = err;//一个极其重要的参数，得出了车体偏离中线的误差值
}

/**
 * @brief 跟踪处理函数
 *
 * 该函数执行图像或信号的跟踪处理流程，包括搜索线条和边缘检测
 *
 * @param void 无参数
 * @return void 无返回值
 */
//总执行函数
void track_proc(void)
{
    // 搜索线条位置
    search_line();
    // 执行边缘检测
    edge_detect();
    //repair_line();
}

