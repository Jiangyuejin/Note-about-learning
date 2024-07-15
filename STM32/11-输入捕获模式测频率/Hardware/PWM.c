#include "stm32f10x.h"                  // Device header


/*
*   函   数：  PWM初始化
*   参   数：  无
*   返回 值:   无
*   描   述:   通过PA0口，使用定时器TIM2
*              以输出比较方式产生PWM
*/
void PWM_Init(void)
{
    /*开启时钟，先开启TIM2，然后是AP0的*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);               //注意GPIOA时钟APB2
    
    /*初始化GPIO*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                     //PA0是复用引脚
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*配置时钟源*/
    TIM_InternalClockConfig(TIM2);
    
    /*时基单元初始化*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //时钟分频，选择不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;     //计数模式，选择向上计数模式
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                     //计数周期，ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;                  //预分频器，psc
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    
    /*输出比较初始化*/
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                   //输出比较模式，选择PWM1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;           //输出极性，选择高，低极性，电平会取反输出
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;       //输出使能
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    
    /*使能*/
    TIM_Cmd(TIM2, ENABLE);
    
}

/*
*       函   数:  PWM设置CCR
*       注   意：      CCR ARR共同决定占空比 = CCR / (ARR + 1)
*/

void PWM_SetCompare1(uint16_t Compare)   
{
    TIM_SetCompare1(TIM2, Compare);  //此函数用于在运动中控制CRR的值，调节占空比
}

/*
*   函   数：  PWM设置PSC
*/
void PWM_SetPrescaler(uint16_t Prescaler)
{
    TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Immediate);
}