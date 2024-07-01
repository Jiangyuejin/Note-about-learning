#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"

int main (void)
{
    /*OLED模块初始化*/
    OLED_Init();
    /*OLED显示*/
    OLED_ShowChar(1, 1, 'A');   // 1行1列显示字母A
    OLED_ShowString(1, 3, "Hello STM32"); //  1行3列显示字符串
    OLED_ShowNum(2, 1, 12345, 5);   //2行1列显示数字12345， 长度为5
    
    while (1)
    {
        
    }
}