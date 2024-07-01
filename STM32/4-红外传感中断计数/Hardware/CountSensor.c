#include "stm32f10x.h"                  // Device header
uint16_t CountSensor_Count;  //全局变量，用于计数

/*
* 函  数: 计数传感器初始化
* 参  数：无
* 返回值： 无
*/

void CountSensor_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //开启GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//开启AFIO时钟
    
    /*初始化GPIO*/
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /*AFIO选择中断引脚*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
    
    /*配置EXTI*/
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line14;               //选择配置外部中断的14号线
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;                 //指定的外部中断线使能
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;       //指定外部中断为中断模型
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;   //指定外部中断为下降沿触发
    EXTI_Init(&EXTI_InitStructure);
    
    /*NVIC中断分组*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    /*NVIC配置，因NVIC为CPU内部设备，不需要开启时钟，位于misc.c*/
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
}

/*
*   函   数： 获取计数值
*   参   数： 无
*   返回值 ： 计数值
*/
uint16_t CountSensor_Get(void)
{
    return CountSensor_Count;
}

/*
*   函   数： EXTI15_10_IRQn
*   参   数： 无
*   返回值 ： 无
*   注意事项：此函数为中断函数，无需调用，中断触发后直接执行
*/
void EXTI15_10_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line14) == SET)  //判断是否是外部中断14号线触发的中断
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == RESET)
        {
            CountSensor_Count++;
        }
        //清除中断标志位
        EXTI_ClearITPendingBit(EXTI_Line14);
    }
}