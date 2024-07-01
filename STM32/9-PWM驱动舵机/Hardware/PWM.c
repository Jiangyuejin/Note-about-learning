#include "stm32f10x.h"                  // Device header


/*
*   函   数：  PWM初始化
*   参   数：  无
*   返回 值:   无
*/
void PWM_Init(void)
{
    /*开启时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                   //使用PA1的复用通道，选择TIM2_CH2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*选择时钟源*/
    TIM_InternalClockConfig(TIM2);
    
    /*时基单元初始化*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1;
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    
    /*输出比较初始化*/
    TIM_OCInitTypeDef TIM_OCInitStructure;
    //给初始化结构体赋值，避免不确定性
    TIM_OCStructInit(&TIM_OCInitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1 ;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    
    //使用的是TIM2的CH2通道
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
    
    /*定时器使能*/
    TIM_Cmd(TIM2, ENABLE);
    
}

/*
*       函   数:  PWM设置CRR
*       注   意：      CRR ARR共同决定占空比 = CRR / (ARR + 1)
*/

void PWM_SetCompare2(uint16_t Compare)   
{
    TIM_SetCompare2(TIM2, Compare);  //此函数用于在运动中控制CRR的值，调节占空比
}