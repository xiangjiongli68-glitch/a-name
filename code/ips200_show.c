#include <ips200_show.h>
#include "track_element.h" // <--- 新增这一行，为了读取 angle_l
/**
 * @brief IPS200显示屏初始化函数
 *
 * 该函数用于初始化IPS200显示屏，设置显示颜色、字体、方向等基本参数
 * 并执行显示屏的初始化操作
 *
 * @param void 无参数
 * @return void 无返回值
 */
void ips200_show_init(void)
{
    ips200_set_color(RGB565_BLACK, RGB565_WHITE);
    ips200_set_font(IPS114_8X16_FONT);
    ips200_set_dir(IPS200_PORTAIT);
    ips200_init(IPS200_TYPE_SPI);
}

/**
 * @brief IPS200显示屏处理函数
 *        显示二值化图像和相关参数信息
 *
 * @param void 无参数
 * @return void 无返回值
 */
void ips200_show_proc(void)
{
    //显示灰度图像
    ips200_show_gray_image(50, 0, bin_image[0], Image_X, Image_Y, Image_X * 2, Image_Y * 2, 0);

    // 显示图像误差值
    ips200_show_string(0, 120, "image_err:");ips200_show_int(80,  120, (int)image_err, 2);

    ips200_show_string(0, 140, "l_pulse:");ips200_show_int(70,  140, (int)l_pulse, 3);

    ips200_show_string(0, 160, "r_pulse:");ips200_show_int(70,  160, (int)r_pulse, 3);

    ips200_show_string(0, 180, "AngL:");       // 显示标签
    ips200_show_int(50, 180, (int)angle_l, 1); // 显示数值 (0或1)

    ips200_show_string(80, 180, "EndRow:");        // 标签
    ips200_show_int(140, 180, (int)line_end_row, 2); // 显示2位数字
    ips200_show_string(0, 200, "FarW:");
    ips200_show_int(50, 200, far_white_count, 3);
    }
    //ips200_show_float(172, 120, total_angle, 3, 3);


    // 显示道路宽度数组的各个元素值
//        ips200_show_int(64, 0, road_wide[0], 3);
//        ips200_show_int(64, 20, road_wide[1], 3);
//        ips200_show_int(64, 40, road_wide[2], 3);
//        ips200_show_int(64, 60, road_wide[3], 3);
//        ips200_show_int(64, 80, road_wide[4], 3);
//        ips200_show_int(64, 100, road_wide[5], 3);
//        ips200_show_int(94, 0, road_wide[6], 3);
//        ips200_show_int(94, 20, road_wide[7], 3);
//        ips200_show_int(94, 40, road_wide[8], 3);
//        ips200_show_int(94, 60, road_wide[9], 3);
//        ips200_show_int(94, 80, road_wide[10], 3);
//        ips200_show_int(94, 100, road_wide[11], 3);
//        ips200_show_int(124, 0, road_wide[12], 3);
//        ips200_show_int(124, 20, road_wide[13], 3);
//        ips200_show_int(124, 40, road_wide[14], 3);
//        ips200_show_int(124, 60, road_wide[15], 3);
//        ips200_show_int(124, 80, road_wide[16], 3);
//        ips200_show_int(124, 100, road_wide[17], 3);
//        ips200_show_int(154, 0, road_wide[18], 3);
//        ips200_show_int(154, 20, road_wide[19], 3);
//        ips200_show_int(154, 40, road_wide[20], 3);
//        ips200_show_int(154, 60, road_wide[21], 3);
//        ips200_show_int(154, 80, road_wide[22], 3);
//        ips200_show_int(154, 100, road_wide[23], 3);
//        ips200_show_int(64, 0, road_wide[24], 3);
//        ips200_show_int(64, 20, road_wide[25], 3);
//        ips200_show_int(64, 40, road_wide[26], 3);
//        ips200_show_int(64, 60, road_wide[27], 3);
//        ips200_show_int(64, 80, road_wide[28], 3);
//        ips200_show_int(64, 100, road_wide[29], 3);
//        ips200_show_int(94, 0, road_wide[30], 3);
//        ips200_show_int(94, 20, road_wide[31], 3);
//        ips200_show_int(94, 40, road_wide[32], 3);
//        ips200_show_int(94, 60, road_wide[33], 3);
//        ips200_show_int(94, 80, road_wide[34], 3);
//        ips200_show_int(94, 100, road_wide[35], 3);
//        ips200_show_int(124, 0, road_wide[36], 3);
//        ips200_show_int(124, 20, road_wide[37], 3);
//        ips200_show_int(124, 40, road_wide[38], 3);
//        ips200_show_int(124, 60, road_wide[39], 3);
//        ips200_show_int(124, 80, road_wide[40], 3);
//        ips200_show_int(124, 100, road_wide[41], 3);
//        ips200_show_int(154, 0, road_wide[42], 3);
//        ips200_show_int(154, 20, road_wide[43], 3);
//        ips200_show_int(154, 40, road_wide[44], 3);
//        ips200_show_int(154, 60, road_wide[45], 3);
//        ips200_show_int(154, 80, road_wide[46], 3);
//        ips200_show_int(154, 100, road_wide[47], 3);

