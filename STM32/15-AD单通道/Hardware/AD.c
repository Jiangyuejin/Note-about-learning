#include "stm32f10x.h"                  // Device header

void AD_Init(void)    
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /*设置ADC时钟*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*规则组通道配置*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 0, ADC_SampleTime_55Cycles5);
    
    /*ADC初始化*/
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  //模式，选择独立模式，其他的是双模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              //数据对齐，右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //外部触发，因使用原件触发，不需要外部触发
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 //连续转换，失能，没转换一次规则组之后就停
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       //扫描模式，失能，只转换规则组序号1这一个位置
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             //通道数，1
    ADC_Init(ADC1, &ADC_InitStructure);
    
    /*ADC使能*/
    ADC_Cmd(ADC1, ENABLE);
    
    /*ADC校准*/
    //固定流程，内部有电路会自动执行校准
    ADC_ResetCalibration(ADC1);                          //ADC复位校准
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);  //获取ADC复位校准状态，判断是否完成
    ADC_StartCalibration(ADC1);                          //  ADC开始校准
    while (ADC_GetCalibrationStatus(ADC1) == SET);       //  获取ADC开始校准状态，判断是否完成
}

/*
*   函   数：获取AD转换值
*   参   数：无
*   返回 值：AD值的转换范围，0-4095
*/

uint16_t AD_GetValue(void)
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);             //软件触发AD转换1次
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);  //等待EOC标志结束， 即AD转换结束
    return ADC_GetConversionValue(ADC1);                        //读数据寄存器，得到AD的转换结果
}