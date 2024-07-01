#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "OLED.h"

uint16_t Num= 0;

int main (void)
{
    /*OLED模块初始化*/
    OLED_Init();
    
    /*定时器2初始化*/
    Timer_Init();
    
    /*OLED显示字符串*/
    OLED_ShowString(1, 1, "Num:"); //  1行3列显示字符串
    OLED_ShowString(2, 1, "CNT:"); //  1行3列显示字符串
   
    while (1)
    {
       OLED_ShowNum(1, 10, Num, 5); //  1行3列显示字符串
       OLED_ShowNum(2, 10, TIM_GetCounter(TIM2), 5); //  1行3列显示字符串 
    }
}

void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)   //判单是否是TIM2更新事件触发中断
    {
        Num++;
        //中断标志位必须清除，否者中断将不断触发导致主程序卡死
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}