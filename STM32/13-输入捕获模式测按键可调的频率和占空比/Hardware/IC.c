#include "stm32f10x.h"                  // Device header


/*
*   函   数：  输入捕获初始化
*   描   述：  使用PA6的复用定时器TIM3
*/

void IC_Init(void)
{
    /*开启时钟，先开启TIM3时钟，然后GPIOA时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*配置时钟源*/
    TIM_InternalClockConfig(TIM3);
    
    /*时基单元初始化,  使用测周法测周期*/
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;             //时钟分频，选择不分频    
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;         //计数模式，选择向上计数模式 
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;                       //计数周期， ARR，以防计数溢出跳到最大
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 -1;                        //预分频器， PSC  fs = 1Mhz
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;                    //重复计数器，高级定时器功能
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
    
    /*输入捕获初始化*/
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                        //选择配置定时器通道，选择通道1
    TIM_ICInitStructure.TIM_ICFilter = 0xF;                                 //输入滤波参数，消除抖动
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;             //极性，选择上升沿触发捕获
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;                   //捕获预分频，选择不分频，每次信号都触发捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;         //输入信号选择，选择直通，不交叉
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_PWMIConfig(TIM3, &TIM_ICInitStructure);
    
    /*选择触发源及从模式*/
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);                            //触发源选择
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);                         //从模式选择复位，TI1产生上升沿，会触发CNT清零
    
    /*TIM使能*/
    TIM_Cmd(TIM3, ENABLE);
}

/*
*   函   数：  获取输入捕获的频率
*/
uint32_t IC_GetFreq(void)
{
    //fs = 1MHz
    return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}

uint32_t IC_GetDuty(void)
{
    //高电平的计数值存在CCR2里,整个周期的计数值存在CCR1里
    return ((TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1));   // 占空比  CCR2 / CCR1;
}