#include "stm32f10x.h"                  // Device header

void AD_Init(void)    
{
    /*开启时钟， ADC1时钟，GPIO时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /*设置ADC时钟*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);           //ADC最大频率14M，72 / 6 = 12MHz
    
    /*GPIOA初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    //不在此处配置规则组初始化，在每次转换前配置，这样使用ADC可以更灵活
    /*ADC初始化*/
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;      //转换模式，选择独立
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;                     //数据对齐，右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;              //外部触发，选择软件触发
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;            //连续转换模式，使能， 每次转换1次后就停止
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                  //扫描模式，失能
    ADC_InitStructure.ADC_NbrOfChannel = 1;                  //通道数
    ADC_Init(ADC1, &ADC_InitStructure);
    
    /*ADC使能*/
    ADC_Cmd(ADC1, ENABLE);
    
    /*ADC校准*/
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
}

/*
*   函   数：获取AD转换值
*   参   数：  ADC_Channel,指定ADC转换的通道，范围：ADC_Channel_x，x值是0/1/2 
*   返回 值：AD值的转换范围，0-4095
*/

uint16_t AD_GetValue(uint8_t ADC_Channel)
{
    ADC_RegularChannelConfig(ADC1, ADC_Channel, 1, ADC_SampleTime_55Cycles5);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);                           //软件触发AD转换1次
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);           //等待EOC标志结束， 即AD转换结束
    return ADC_GetConversionValue(ADC1);                              //读数据寄存器，得到AD的转换结果
}