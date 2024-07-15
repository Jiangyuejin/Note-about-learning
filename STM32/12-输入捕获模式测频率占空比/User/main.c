#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"

uint8_t i= 0;

int main (void)
{
    /*模块初始化*/
    OLED_Init();
    PWM_Init();
    IC_Init();
    
    /*显示静态字符串*/
    OLED_ShowString(1, 1, "Freq:00000Hz");
    OLED_ShowString(2, 1, "Duty:0000");
    
    /*使用PWM模块提供捕获输入的信号*/
    PWM_SetPrescaler(720 - 1);          //PWM频率 Freq = 72M / (PSC + 1) / 100
    PWM_SetCompare1(40);                //PWM占空比 Duty = CCR /100
 
    while (1)
    {
        OLED_ShowNum(1, 6, IC_GetFreq(), 5);
        OLED_ShowNum(2, 6, IC_GetDuty(), 4);
    }
  
}

