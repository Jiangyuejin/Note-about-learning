#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "PWM.h"
#include "IC.h"
#include "Key.h"

/*做个拓展，通过2个按键控制占空比和频率*/

uint8_t i= 0;
uint32_t PSC = 72;
uint8_t CCR = 10;

int main (void)
{
    /*模块初始化*/
    OLED_Init();
    PWM_Init();
    IC_Init();
    Key_Init();
    
    /*显示静态字符串*/
    OLED_ShowString(1, 1, "Freq:00000Hz");
    OLED_ShowString(2, 1, "Duty:");
    
    /*使用PWM模块提供捕获输入的信号*/
    
    PWM_SetCompare1(CCR);                //PWM占空比 Duty = CCR /100
    PWM_SetPrescaler(PSC - 1);          //PWM频率 Freq = 72M / (PSC + 1) / 100
 
    while (1)
    {
        /*改变频率*/
        uint8_t key = 0;
        key = Key_GetNum();
        
        if (key == 1)   
        {
            PSC *= 10;      //只能最大是7200
            if (PSC > 7200 )  //0---65535
            {
                PSC = 72;
            }
            PWM_SetPrescaler(PSC - 1);          //PWM频率 Freq = 72M / (PSC + 1) / 100
        }
        if (key == 2)
        {
            CCR += 10;
            if (CCR > 90)  
            {
                CCR = 10;
            }
            PWM_SetCompare1(CCR);     
        }
        
        OLED_ShowNum(1, 6, IC_GetFreq(), 5);
        OLED_ShowNum(2, 6, IC_GetDuty(), 5);
        
    }
  
}

