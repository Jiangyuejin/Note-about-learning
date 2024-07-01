#include "stm32f10x.h"                  // Device header

void Sensor_Init(void)
{
    //开启时钟，GPIO和AFIO
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    
    //初始化GPIO
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = ;
    GPIO_InitStructure.GPIO_Pin = ;
    GPIO_InitStructure.GPIO_Speed = ;
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    //AFIO选择中断引脚
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource14)
    
    //外部中断EXTI初始化
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = ;
    EXTI_InitStructure.EXTI_LineCmd = ;
    EXTI_InitStructure.EXTI_Mode = ;
    EXTI_InitStructure.EXTI_Trigger = ;
    
    EXTI_Init(&EXTI_InitStructure);
    
    //NVIC中断分组
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    //NVIC配置
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = ;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = ;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = ;
    NVIC_Init(&NVIC_InitStructure);
}