#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"

uint8_t i= 0;

int main (void)
{
    /*模块初始化*/
    OLED_Init();
    PWM_Init();
    
 
    while (1)
    {
        //OLED_ShowString(1, 1, "what ！？");   //为了进行调试
        for (i = 0; i <= 100; i++) 
        {
            PWM_SetCompare1(i);    //占空比从0到100，逐渐变大，灯逐渐变亮
            Delay_ms(10);
        }
        for (i = 0; i <= 100; i++) 
        {
            PWM_SetCompare1(100 - i);  //占空比从100逐渐减小到0， 逐渐变小，灯逐渐变暗
            Delay_ms(10);
        }
    }
}

