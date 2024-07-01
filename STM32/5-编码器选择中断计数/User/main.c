#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Encoder.h"
#include "OLED.h"

int16_t Num = 0;

int main (void)
{
    /*OLED模块初始化*/
    OLED_Init();
    
    /*编码器初始化*/
    Encoder_Init();
    
    /*OLED显示字符串*/
    OLED_ShowString(1, 1, "Num:"); //  1行3列显示字符串
   
    while (1)
    {
        Num += Encoder_Get();
        OLED_ShowSignedNum(1, 5, Num, 5);
    }
}