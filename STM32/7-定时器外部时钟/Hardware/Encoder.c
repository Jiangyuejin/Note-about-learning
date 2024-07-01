#include "stm32f10x.h"                  // Device header
int16_t Encoder_Count = 0;

/*
*   函   数：初始化编码器
*   参   数：无
*   返回值 ：无
*/
void Encoder_Init(void)
{
    //开启APB2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStruct;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    /*AFIO设置选择中断引脚*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0); //将外部中断0号引脚映射到GPIOB
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource1); //将外部中断1号引脚映射到GPIOB
    
    /*EXTI初始化设置*/
    EXTI_InitTypeDef EXTI_InitStruct;
    EXTI_InitStruct.EXTI_Line = EXTI_Line0 | EXTI_Line1;
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_Init(&EXTI_InitStruct);
    
    
    /*NVIC中断分组*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    /*NVIC配置*/
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;             //NVIC线路使能
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;   //NVIC线路的抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;          //NVIC线路的响应优先级
    NVIC_Init(&NVIC_InitStruct);
    
    NVIC_InitStruct.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
    NVIC_Init(&NVIC_InitStruct);
    
}
/*
*   函   数：返回编码器的增量
*/
uint16_t Encoder_Get(void)
{
    uint16_t temp;
    temp = Encoder_Count;
    Encoder_Count = 0;
    return temp;
}
/*
*   函   数：  EXTI0外部中断函数
*/
void EXTI0_IRQHandler(void)
{
    
    if (EXTI_GetITStatus(EXTI_Line0) == SET)    //判断是否是外部中断0号线触发的中断
    {
        
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)  == 0)  //判断一下引脚电平，免得数据乱跳
        {
            //PB0下降沿触发中断，如果PB1为高电平，那就是正转；如果PB1为低电平，那就是反转
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)  == 0) //PB0下降沿触发中断，此时检测PB1电平
            {
                Encoder_Count--;  
            }
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line0);
}

/*
*   函   数：  EXTI0外部中断函数
*/
void EXTI1_IRQHandler(void)
{
    
    if (EXTI_GetITStatus(EXTI_Line1) == SET)    //判断是否是外部中断0号线触发的中断
    {
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1)  == 0)  //判断一下引脚电平，免得数据乱跳
        {
            
            if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)  == 0) //PB0下降沿触发中断，此时检测PB1电平
            {
                Encoder_Count ++;  
            }
        }
    }
    EXTI_ClearITPendingBit(EXTI_Line1);
}