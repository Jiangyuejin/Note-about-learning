#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Timer.h"
#include "OLED.h"
#include "Encoder.h"

int16_t Speed= 0;

int main (void)
{
    /*OLED模块初始化*/
    OLED_Init();
    Timer_Init();
    Encoder_Init();
    
    /*OLED显示字符串*/
    OLED_ShowString(1, 1, "Speed:"); //  1行1列显示字符串
    //OLED_ShowString(2, 1, "CNT:"); //  1行1列显示字符串

   
    while (1)
    {
       OLED_ShowSignedNum(1, 7, Speed, 5); //  1行3列显示字符串
//       OLED_ShowSignedNum(2, 6, Encoder_Get(), 5); //  1行3列显示字符串
        
    }
}

/*
*   函   数：中断返回编码器的速度
*   描   述：编码器测速度，可以在固定的闸门时间读一次CNT，然后把CNT清零，
*            此时CNT的值代表速度，单位是脉冲个数/S， 此定时器闸门1s
*/
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        Speed = Encoder_Get();
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}