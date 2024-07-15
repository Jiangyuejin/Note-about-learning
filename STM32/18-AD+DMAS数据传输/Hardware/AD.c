#include "stm32f10x.h"                  // Device header

uint16_t AD_Value[4];                   //定义用于存放AD转换结果的全局数组

void AD_Init(void)    
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    /*设置ADC时钟*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
    
    /*初始化GPIOA*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*规则组通道配置*/
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);
    
    /*ADC初始化*/
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;      //模式，选择独立模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;  //数据对齐， 右对齐
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //外部触发，使用软件触发模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;      //连续转换，使能， 没转换一次规则组序列后立刻开始下一次转换
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;            //扫描模式， 使能，扫描规则组的序列，扫描数量由ADC_NbrOfChannel确定
    ADC_InitStructure.ADC_NbrOfChannel = 4;                 //通道数 4， 扫描规则组前四个通道
    ADC_Init(ADC1, &ADC_InitStructure);
    
    /*DMA初始化*/
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;             //外设基地址
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据宽度，半个字节，对应16为ADC数据寄存器
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //外设地址自增，选择失能，始终以ADC数据寄存器为源
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;                  //寄存器基地址
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //寄存器数据宽度，半个字节
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //存储器地址自增，使能，每次转运之后数组移到下一个位置
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          //数据传输方向， 外设为源地址
    DMA_InitStructure.DMA_BufferSize = 4;                                       //转运数据大小，与ADC通道一致
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             //模式，选择循环模式，与ADC的连续转换一致
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                //存储器到存储器， 失能， 由ADC外设触发转运到存储器
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       //优先级，中等优先级
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    
    /*DMA和ADC使能*/
    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);                       //ADC1触发DMA1的信号使能
    ADC_Cmd(ADC1, ENABLE);
    
    
    /*ADC校准*/
    ADC_ResetCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
    
    /*ADC触发*/
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    
}

