#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/*
*   函   数：  舵机初始化
*   返回值：    无
*/
void Servo_Init(void)
{
    PWM_Init();
}

/*
*   函   数：  舵机设置角度
*   参   数：  Angle要设置舵机角度，范围0~180
*   返回 值：
*/
void Servo_SetAngle(float Angle)
{
    PWM_SetCompare2(Angle / 180 * 2000 + 500);
}