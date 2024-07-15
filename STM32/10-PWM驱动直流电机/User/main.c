#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"

uint8_t KeyNum = 0;
int8_t Speed = 0;

int main (void)
{
    /*模块初始化*/
    OLED_Init();
    Motor_Init();
    Key_Init();
    
    /*显示静态字符串*/
    OLED_ShowString(1, 1, "Speed: ");
    while (1)
    {
        KeyNum = Key_GetNum();              //获取按键的码1-->对应PB1
        if (KeyNum == 1)
        {
            Speed += 20;                    //速度增20
            if (Speed > 100)
            {
                Speed = 0;               //速度超过100，让他停
            }
        }
        if (KeyNum == 2)
        {
            Speed -= 20;                    //速度增20
            if (Speed < 0)
            {
                Speed = 0;               //速度超过100，让他停
            }
        }
        /*设置电机的转速*/
        Motor_SetSpeed(Speed);
        /*显示一下转速*/
        OLED_ShowSignedNum(1, 7, Speed, 3);
    }
}

