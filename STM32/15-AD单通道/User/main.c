#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "AD.h"

uint16_t ADValue = 0;           //定义AD值变量
float Voltage = 0.0;            //定义电压值变量

int main (void)
{
    /*OLED模块初始化*/
    OLED_Init();
    AD_Init();
    
    /*OLED显示*/
    
    OLED_ShowString(1, 1, "ADValue:"); //  1行3列显示字符串
    OLED_ShowString(2, 1, "Voitage:0.00V"); //  1行3列显示字符串
    
    while (1)
    {
        ADValue = AD_GetValue();                        //获取AD转换值
        Voltage = (float) ADValue / 4095 * 3.3;         //将AD值线性转换到0-3.3V范围，表示电压
        
        OLED_ShowNum(1, 9, ADValue, 4);                 //显示AD值
        OLED_ShowNum(2, 9, Voltage, 1);                 //显示电压整数部分
        OLED_ShowNum(2,11, (uint16_t)(Voltage * 100) % 100, 2);     //显示电压小数部分
        
        Delay_ms(100);
    }
}