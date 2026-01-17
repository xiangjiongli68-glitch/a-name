/*
 * Functiom.c
 *作用：限幅，保护
 *  Created on: 2025年12月17日
 *      Author: Dengke Sun
 */
//只有保护作用
#include "Function.h"

/******************************************************************
//      @function:Limit_int( int a, int b, int c )
//      Description:限制大小范围a-c，b为输入
//      Input: a(下限/Min), b(输入值/Value), c(上限/Max)
//      Output:
//      Return: 限制后的int值
//      Others:
*******************************************************************/
int Limit_int(int a, int b, int c)
{
    // 如果 b 在 [a, c] 范围内，直接返回 b
    if ((b >= a) && (b <= c))
    {
        return b;
    }
    // 如果 b 小于下限 a，返回 a
    else if (b < a)
    {
        return a;
    }
    // 如果 b 大于上限 c，返回 c
    else if (b > c)
    {
        return c;
    }

    // 为了防止编译器警告或作为逻辑保底
    return 0;
}

/******************************************************************
//      @function:Limit_uint8( uint8 a, uint8 b, uint8 c )
//      Description:限制大小范围a-c，b为输入
//      Input:
//      Output:
//      Return:
//      Others:
*******************************************************************/
uint8 Limit_uint8(uint8 a, uint8 b, uint8 c)
{
    if ((b >= a) && (b <= c))
    {
        return b;
    }
    else if (b < a)
    {
        return a;
    }
    else if (b > c)
    {
        return c;
    }
    return 0;
}

/******************************************************************
//      @function:Limit_float( float a, float b, float c )
//      Description:限制大小范围a-c，b为输入
//      Input: a(下限/Min), b(输入值/Value), c(上限/Max)
//      Output:
//      Return: 限制后的float值
//      Others:
*******************************************************************/
float Limit_float(float a, float b, float c)
{
    // 如果 b 在 [a, c] 范围内，直接返回 b
    if ((b >= a) && (b <= c))
    {
        return b;
    }
    // 如果 b 小于下限 a，返回 a
    else if (b < a)
    {
        return a;
    }
    // 如果 b 大于上限 c，返回 c
    else if (b > c)
    {
        return c;
    }

    return 0.0f;
}


