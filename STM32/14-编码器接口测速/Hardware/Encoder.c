#include "stm32f10x.h"                  // Device header
int16_t Encoder_Count = 0;

/*
*   函   数：初始化编码器
*   参   数：无
*   返回值 ：无
*   描   述：使用TIM3的CH1 和 CH2
*/
void Encoder_Init(void)
{
    /*开启时钟，包括TIM3时钟、GPIOA时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 |GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*时基单元初始化*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;             //时钟分频，选择
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //计数器模式，选择向上计数
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;                       //计数周期，ARR
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                        //预分频器，PSC
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                    //重复计数器，高级定时器功能
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    /*输入捕获初始化*/
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);                                 //避免结构体初值不确定，给结构体初始化
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                        //选择配置定时器通道1
    TIM_ICInitStructure.TIM_ICFilter = 0xF;                                 //滤波器参数
    TIM_ICInit(TIM3, &TIM_ICInitStructure);                                 
    
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICFilter = 0xF;
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    
    /*编码器接口配置*/
    //此函数必须在捕获初始化之后进行，此函数参数Falling Rising不代表下降沿、上升沿，代表是否反向
    //Rising----------》高低电平不反转， Falling----------------》高低电平反转
    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    
    /*TIM使能*/
    TIM_Cmd(TIM3, ENABLE);
   
}
/*
*   函   数：返回编码器的增量
*/
int16_t Encoder_Get(void)
{
    int16_t temp;
    temp = TIM_GetCounter(TIM3);
    TIM_SetCounter(TIM3, 0);
    return temp;
}

/*
*   函   数：返回编码器的增量(只返回位置)
*/
//int16_t Encoder_Get(void)
//{
//    return TIM_GetCounter(TIM3);
//}

