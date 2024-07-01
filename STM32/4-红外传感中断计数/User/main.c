#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "CountSensor.h"
#include "OLED.h"

int main (void)
{
    /*OLED模块初始化*/
    OLED_Init();
    
    /*计数传感器初始化*/
    CountSensor_Init();
    
    /*OLED显示字符串*/
    OLED_ShowString(1, 1, "Count:"); //  1行3列显示字符串
   
    while (1)
    {
        OLED_ShowNum(1, 7, CountSensor_Get(), 5);
    }
}