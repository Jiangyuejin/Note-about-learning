#include "stm32f10x.h"                  // Device header

/**
  * 函    数：定时中断初始化(采用内部时钟)
  * 参    数：无
  * 返 回 值：无
  */
void Timer_Init(void)
{
	/*开启时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);			//开启TIM2的时钟
	
	/*配置时钟源*/
	TIM_InternalClockConfig(TIM2);		//选择TIM2为内部时钟，若不调用此函数，TIM默认也为内部时钟
	
	/*时基单元初始化*/  
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;                  //定义结构体变量
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //时钟分频，选择不分频，此参数用于配置滤波器时钟，不影响时基单元
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;     //计数器模式，选择向上计数
    TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;                   //计数周期，ARR值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;                 //预分频器，PSC值
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                 //重复计数器，高级定时器用到
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    
    /*中断输出配置*/
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);         //清除定时器更新标志
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);    //开启TIM2的更新中断
    
    /*NVIC中断分组*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    /*NVIC配置*/
    NVIC_InitTypeDef NVIC_InitStruct;
    NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;            //选择配置NVIC的TIM2线
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;            //指定的NVIC线使能
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;  //指定的NVIC线的抢占优先级
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;         //指定的NVIC线的响应优先级
    NVIC_Init(&NVIC_InitStruct);
    
    /*TIM2使能*/
    TIM_Cmd(TIM2, ENABLE);
   
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
