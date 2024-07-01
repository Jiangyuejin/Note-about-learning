#include "stm32f10x.h"                  // Device header


/*
*   函   数：  PWM初始化
*   参   数：  无
*   返回 值:   无
*/
void PWM_Init(void)
{
    /*开启时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);        //开启定时器TIM2时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);       //开启GPIOA时钟 复制出问题啦
    
    /*GPIO重映射功能，将GPIOA15引脚设置有能使用TIM2_CH1*/
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);       //开启AFIO时钟
//    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);      //将TIM2的引脚部分重映射
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);   //JTAG脚使能作为普通GPIO引脚使用
    
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                 //GPIO_Pin_15
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*开启时钟源*/
    TIM_InternalClockConfig(TIM2);      //使用内部时钟
    
    /*时基单元初始化, 此处设置的频率为1KHz,Ts = 1ms*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;             //时钟分频，选择不分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //计数器模式，选择向上计数
    TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;                         //计数周期，ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1;                      //预分频器，PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                    //重复计数器，高级定时器功能
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
    
    /*输出比较初始化*/
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCStructInit(&TIM_OCInitStructure);
    
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;               //输出比较模式，选择PWM模式1
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;       //输出极性，选择高，若为低，则输出高低电平取反
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;   //输出使能
    TIM_OCInitStructure.TIM_Pulse = 0;                              //初始化CRR,控制占空比
    TIM_OC1Init(TIM2, &TIM_OCInitStructure);
    
    /*TMI使能*/
    TIM_Cmd(TIM2, ENABLE);
}

/*
*       函   数:  PWM设置CRR
*       注   意：      CRR ARR共同决定占空比 = CRR / (ARR + 1)
*/

void PWM_SetCompare1(uint16_t Compare)   
{
    TIM_SetCompare1(TIM2, Compare);  //此函数用于在运动中控制CRR的值，调节占空比
}