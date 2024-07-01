#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "Servo.h"
#include "Key.h"

uint8_t KeyNum;
float Angle;

int main (void)
{
    /*模块初始化*/
    OLED_Init();
    PWM_Init();
    Servo_Init();
    Key_Init();
    
    /*显示静态字符串*/
    OLED_ShowString(1, 1, "Angle:");
 
    while (1)
    {
        KeyNum = Key_GetNum();  //获取按键值
        if (KeyNum == 2)    //若是按键
        {
            Angle += 30;   //角度增加30
            if (Angle > 180)
            {
                Angle = 0;
            }
        }
        if (KeyNum == 1)    //若是按键
        {
            Angle -= 30;   //角度增加30
            if (Angle < 0)
            {
                Angle = 180;
            }
        }
        //舵机设置角度
        Servo_SetAngle(Angle);
        OLED_ShowNum(1, 7, Angle, 3);
    }
       
}

