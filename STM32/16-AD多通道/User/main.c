#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t AD0, AD1, AD2;           //定义AD值变量
float Voltage0, Voltage1;            //定义电压值变量

int main (void)
{
    /*OLED模块初始化*/
    OLED_Init();
    AD_Init();
    
    /*OLED显示*/
    
    OLED_ShowString(1, 1, "AD0:0.00V");      //  1行1列显示字符串
    OLED_ShowString(2, 1, "AD1:0.00V");      //  2行1列显示字符串
    OLED_ShowString(3, 1, "AD2:");      //  3行1列显示字符串
    
    while (1)
    {
        AD0 = AD_GetValue(ADC_Channel_0);                        //获取AD转换值
        Voltage0 = (float) AD0 / 4095 * 3.3;         //将AD值线性转换到0-3.3V范围，表示电压
        
        AD1 = AD_GetValue(ADC_Channel_1);                        //获取AD转换值
        Voltage1 = (float) AD1 / 4095 * 3.3;         //将AD值线性转换到0-3.3V范围，表示电压
        AD2 = AD_GetValue(ADC_Channel_2);  
        
        OLED_ShowNum(1, 5, Voltage0, 1);                 //显示电压整数部分
        OLED_ShowNum(1, 7, (uint16_t)(Voltage0 * 100) % 100, 2);     //显示电压小数部分
        
        OLED_ShowNum(2, 5, Voltage1, 1);                 //显示电压整数部分
        OLED_ShowNum(2, 7, (uint16_t)(Voltage1 * 100) % 100, 2);     //显示电压小数部分
        
        OLED_ShowNum(3, 9, AD2, 4);   
        
        Delay_ms(100);
    }
}