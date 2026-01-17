//将摄像头拍到的画面，抽象成可识别的地图
#include "image.h"

int32_t THRESHOLD = 125;

uint8 bin_image[Image_Y][Image_X];//二值化后的黑白图
uint8 crop_image[Image_Y][Image_X];//灰度图
int far_white_count = 0;
/**
 * @brief 图像裁剪函数
 * @description 从MT9V03X图像传感器的原始图像数据中裁剪出指定区域的图像
 *              裁剪起始位置为(14, 5)，裁剪尺寸为Image_X * Image_Y
 * @param 无参数
 * @return 无返回值
 */
//图像裁剪
static void image_crop()
{
    uint8 i;

    // 将原始图像数据从起始位置(14, 5)开始按行复制到裁剪图像缓冲区
    for (i = 0; i < Image_Y; i++)
    {
        memcpy(crop_image[i], &mt9v03x_image[5 + i][14], Image_X);
    }
}

/**
 * @brief 图像二值化处理函数
 * @param threshold 二值化阈值，用于判断像素点是白色还是黑色
 * @return 无返回值
 *
 * 该函数将原始图像数组crop_image按照指定阈值进行二值化处理，
 * 结果存储到二值化图像数组bin_image中
 */
//将五彩斑斓的灰简化成纯黑/纯白
void image_binary(uint8 threshold)
{
    uint8 i, j;

    // 遍历图像的每一行和每一列，进行二值化处理
    for (i = 0; i < Image_Y; i++)
    {
        for (j = 0; j < Image_X; j++)
        {
            if (crop_image[i][j] >= threshold)
                bin_image[i][j] = White;
            else
                bin_image[i][j] = Black;
        }
    }

    // 重置MT9V03X图像传感器完成标志位
    mt9v03x_finish_flag = 0;
}
void Calculate_Far_Pixels(void)
{
    uint8 i, j;
    int count = 0;

    // 遍历图像最顶部的 10 行
    for(i = 0; i < 10; i++)
    {
        for(j = 0; j < Image_X; j++)
        {
            if(bin_image[i][j] == White)
            {
                count++;
            }
        }
    }
    far_white_count = count;
}
/**
 * @brief 图像处理函数
 * @description 对图像进行裁剪和二值化处理
 * @param 无参数
 * @return 无返回值
 */
//总处理，实际调用前两个函数
void image_proc(void)
{
    // 执行图像裁剪操作
    image_crop();
    // 执行图像裁剪操作
    image_binary(THRESHOLD);
    Calculate_Far_Pixels();
}
