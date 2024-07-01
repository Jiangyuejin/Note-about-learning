#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/*按键初始化函数*/
void Key_Init(void)
{
    //开启时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    //GPIO初始化
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  //上拉输出
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


//获取按键值
uint8_t Key_GetNum(void)
{
    //定义返回的变量，默认值为0
    uint8_t KeyNum = 0;
    
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
    {
        //延时消抖
        Delay_ms(20);
        //等待按键松手
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0);
        //再次延时消抖
        Delay_ms(20);
        //按键码为1
        KeyNum = 1;   
    }
    
    if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
    {
        //延时消抖
        Delay_ms(20);
        //等待按键松手
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);
        //再次延时消抖
        Delay_ms(20);
        //按键码为2
        KeyNum = 2;   
    }
    
    return KeyNum;
}