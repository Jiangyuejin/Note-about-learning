#include "stm32f10x.h"                  // Device header

/**
  * 函    数：定时中断初始化(采用外部时钟)
  * 参    数：无
  * 返 回 值：无
  */
void Timer_Init(void)
{
    /*开启时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  //开启定时器TIM2时钟
    RCC_APB1PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);  //开启GPIOA时钟,然后进行初始化
    
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*配置时钟源*/
    //选择外部时钟模式2，时钟从TIM_ETR输入，固定引脚，无法随意更改
    TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_Inverted, 0x0F);
    
    /*初始化时基单元*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;        //时钟分频，选择不分配
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;    // 计数器模式，选择向上计数
    TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;                     //计数周期，ARR = 10
    TIM_TimeBaseInitStructure.TIM_Prescaler = 3 - 1;                   //预分频器，PSC的值（CNT） ，相当于遮挡几次才会加1次
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;               // 重复计数器，高级计时器才能用到
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    
    /*中断输出设置*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_ITConfig(TIM2, TIM_FLAG_Update, ENABLE);
    
    /*设置NVIC分组*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    /*配置NVIC*/
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    /*定时器使能*/
    TIM_Cmd(TIM2, ENABLE);
}

/*
*   函   数   ：返回定时器的CNT值
*/
uint16_t Timer_GetCounter(void)
{
    //返回计数值
    return TIM_GetCounter(TIM2);
}

/* 定时器中断函数，可以复制到使用它的地方
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
*/
