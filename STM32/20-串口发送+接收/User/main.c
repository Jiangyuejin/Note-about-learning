#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"

uint8_t RxData;
int main (void)
{
    /*OLED模块初始化*/
    OLED_Init();
    Serial_Init();
    
    /*显示静态字符串*/
    OLED_ShowString(1, 1, "RxData:");
   
    while (1)
    {
        if (Serial_GetRxFlag() == 1)
        {
            RxData = Serial_GetRxData();
            Serial_SendByte(RxData);
            OLED_ShowHexNum(1, 8, RxData, 2);
        }
    }
}