#include "stm32f10x.h"                  // Device header

/*LED初始化*/
void LED_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /*初始化GPIO结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*设置上电后的初始状态*/
    GPIO_SetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1); 
}

/*LED0开启*/
void LED0_ON(void) 
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_0);

}

/*LED0开启*/
void LED0_OFF(void) 
{
    GPIO_SetBits(GPIOA, GPIO_Pin_0);
}

/*LED0灯进行反转*/
void LED0_Turn(void)
{
    //获取当前寄存器状态, 如果当前引脚为低电平
    if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_0) == 0) 
    {
         GPIO_SetBits(GPIOA, GPIO_Pin_0);
    }
    else
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
    }
}

/*LED1开启*/
void LED1_ON(void) 
{
    GPIO_ResetBits(GPIOA, GPIO_Pin_1);

}

/*LED1开启*/
void LED1_OFF(void) 
{
    GPIO_SetBits(GPIOA, GPIO_Pin_1);
}

/*LED1灯进行反转*/
void LED1_Turn(void)
{
    //获取当前寄存器状态, 如果当前引脚为低电平
    if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_1) == 0) 
    {
         GPIO_SetBits(GPIOA, GPIO_Pin_1);
    }
    else
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
    }
}