//主控制循环，一直循环
#include "track_element.h"

int32_t WAVY_LINE = 15; // 连续折线部分，如果误判为直角，把这个值改小
int32_t BLANK_LINE = 8; // 如果提前识别到直角，把这个值增大

uint8 element = 0;
uint8 angle_l = 0, angle_r = 0;
uint8 island_l = 0, island_r = 0;
uint8 line_end_row = 0; // 记录线消失的行号
uint16 detection_block_timer = 0;//退出直角后的屏蔽检测
/**
 * @brief 元素判断函数，用于识别图像中的特定元素（如角度、岛屿等）
 *
 * 该函数通过分析二值化图像中的边缘信息，判断是否存在特定的元素类型，
 * 包括左侧和右侧的角点和岛屿元素检测。
 *
 * @param 无参数
 * @return 无返回值
 */
//这个函数用于识别赛道中的特殊元素，比如说直角弯和环岛
void element_judge(void)
{
    // 1. 每次必须清零
    line_end_row = 0;

    uint8 i, j;
    uint8 has_bottom_line = 0;

    // 冷却或已识别则跳过
    if (detection_block_timer > 0)
    {
        detection_block_timer--;
        return;
    }
    if (element) return;

    // ============================================================
    // 【新增核心逻辑】远视眼过滤：检查图像顶部是否有路
    // ============================================================
    uint8 road_at_top = 0;

    // 扫描图像最顶部区域 (比如第 0 到 10 行)
    // 如果在这里还能找到连贯的白线，说明路还在延伸，底下的肯定是干扰
    for (i = 0; i < 10; i++)
    {
        // 在中线附近找白点
        if (bin_image[i][Image_X/2] == White ||
            bin_image[i][Image_X/2 - 2] == White ||
            bin_image[i][Image_X/2 + 2] == White)
        {
            road_at_top = 1;
            break; // 只要顶部有线，就标记为“前方有路”
        }
    }

    // ============================================================
    // 如果顶部有路 (road_at_top == 1)，说明是干扰或十字，
    // 绝对不能识别为直角！直接退出函数！
    // ============================================================
    if (road_at_top == 1)
    {
        return; // 这样可以完美过滤掉你图片里所有的元器件干扰
    }


    // ============================================================
    // 下面是原有的直角检测逻辑 (只有当顶部没路时才会执行到这里)
    // ============================================================

    // 2. 寻找断点 (从下往上)
    for (i = Image_Y - 2; i > 5; i--)
    {
        if (bin_image[i][mid_line[i]] == White ||
            bin_image[i][mid_line[i]-1] == White ||
            bin_image[i][mid_line[i]+1] == White)
        {
            has_bottom_line = 1;
        }
        else
        {
            if (has_bottom_line == 1)
            {
                line_end_row = i;
                break;
            }
        }
    }

    // 3. 判断特征
    if (line_end_row > 10 && line_end_row < 53)
    {
        uint8 left_white_count = 0;
        uint8 right_white_count = 0;

        // 扫描左侧特征
        for(j = 2; j < Image_X / 2 - 2; j++) {
            if(bin_image[line_end_row][j] == White) left_white_count++;
            if(bin_image[line_end_row-3][j] == White) left_white_count++;
            if(bin_image[line_end_row-6][j] == White) left_white_count++;
        }

        // 扫描右侧特征
        for(j = Image_X / 2 + 2; j < Image_X - 2; j++) {
            if(bin_image[line_end_row][j] == White) right_white_count++;
            if(bin_image[line_end_row-3][j] == White) right_white_count++;
            if(bin_image[line_end_row-6][j] == White) right_white_count++;
        }

        // 阈值判定
        if (left_white_count > 5 && right_white_count < 5)
        {
            angle_l = 1;
            element = 1;
        }
        else if (right_white_count > 5 && left_white_count < 5)
        {
            angle_r = 1;
            element = 1;
        }
    }
}

double target_angle = 0;
uint8 angle_flag = 0;
uint8 island_in = 0;
static uint16 turn_counter = 0;//实际的旋转时间
/**
 * @brief 处理特殊赛道元素的函数，包括左直角、右直角、左环岛、右环岛的处理逻辑
 *
 * 该函数根据不同的赛道元素标志（angle_l, angle_r, island_l, island_r）来处理
 * 车辆在遇到特殊赛道元素时的转向和路径规划，包括角度控制、中线调整等操作
 *
 * @param void 无参数
 * @return void 无返回值
 */
//特殊元素的处理函数，用来处理直角和环岛
void element_special_proc(void)
{
    uint8 i;
    if (angle_l == 0 && angle_r == 0) return;

        /******************** 左直角处理 ********************/
        if (angle_l == 1)
        {
            // 1. 刚进入直角的初始化
            if (angle_flag == 0)
            {
                turn_counter = 0; // 清零实际记录旋转时间的计时器
                angle_flag = 1;   // 标记正在处理
            }

            turn_counter++; // 计时

            // 2. 退出判断：先看时间，再看图像
            // 逻辑：只有当“盲转时间够了” 且 “眼睛看到线在中间了”，才算转完
            uint8 find_line = 0;//没看到线标志位

                    if (turn_counter > BLIND_TIME)
                    {
                        // 【核心修改】左转时，线从左边出来。
                        // 我们不查中间，改为查“左半边” (例如 5 到 32)
                        // 只要线一露头，马上退出暴力转向，交给PID去柔和处理
                        for (i = 5; i < Image_X / 2 + 5; i++)
                        {
                            if (bin_image[Image_Y - 5][i] == White)
                            {
                                find_line = 1;
                                break;
                            }
                        }

                // 如果看到线了，说明转正了 -> 退出直角模式
                if (find_line == 1)
                {
                    angle_l = 0;       // 清除左标志
                    angle_flag = 0;    // 清除过程标志
                    element = 0;       // 清除元素标志
                    turn_counter = 0;  // 重置计时器

                    // 【关键一步】强制清除识别变量，防止连转时的误判
                    // 确保您在 track_element.h 里 extern 了这个变量
                    line_end_row = 0;

                    detection_block_timer = 30;

                    return; // 本次循环结束，下一帧恢复正常巡线
                }
            }

            // 3. 执行动作：如果没退出，就继续强行左转
            // 此时我们还没退出，所以要覆盖 mid_line 让 PID 拼命转
            for (i = 0; i < Image_Y; i++)
            {
                mid_line[i] = 0; // 强制线在最左边，image_err 达到最大负值
            }
        }

}
//处理十字路口/环岛的特殊情况
void element_proc(void)
{
    element_judge();
    element_special_proc();
}
