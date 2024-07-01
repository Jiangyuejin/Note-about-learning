#include "stm32f10x.h"                  // Device header
#include "Delay.h"

int main(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /*GPIO_A初始化*/
    GPIO_InitTypeDef GPIO_Initstructure;
    
    //定义结构体变量
    GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP; 
    GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOA, &GPIO_Initstructure);
    
    while(1) 
    {
        /*方式一：GPIO_ResetBits设置低电平，GPIO_SetBits设置高电平*/
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        Delay_ms(1000);
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
        Delay_ms(1000);
        
        /*方式二：通过GPIO_WriteBites设置高低电平， Bit_RESET/Bit_SET指定*/
//        GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
//        Delay_ms(500);
//        GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);
//        Delay_ms(500);
        
        /*方式三：通过GPIO_WriteBites设置高低电平， 由数据0/1指定，数据强制类型转换用BitAction*/
//        GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)0);
//        Delay_ms(500);
//        GPIO_WriteBit(GPIOA, GPIO_Pin_0, (BitAction)1);
//        Delay_ms(500);
    }
    
}