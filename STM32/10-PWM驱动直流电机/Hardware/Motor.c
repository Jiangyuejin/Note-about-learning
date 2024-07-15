#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/*
*   函   数: 直流电机初始化
*   参   数：  无
*   返回值:    无
*/
void Motor_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /*初始化GPIOA*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*初始化PWM*/
    PWM_Init();
}

/*
*   函   数:  直流电机设置转速
*   参   数：  Speed要设置的速度，范围：-100~100
*   返回 值：   无
*/
void Motor_SetSpeed(int8_t Speed)
{
    
    if (Speed >= 0)                             //如果设置正转的速度
    {
        GPIO_SetBits(GPIOA, GPIO_Pin_4);        //PA4设置高电平
        GPIO_ResetBits(GPIOA, GPIO_Pin_5);      //PA5设置低电平，电机正传
        PWM_SetCompare3(Speed);                 //PWM设置转速                
    }
    else                                        //电机反转
    {
        GPIO_ResetBits(GPIOA, GPIO_Pin_4);      //PA4设置低电平
        GPIO_SetBits(GPIOA, GPIO_Pin_5);        //PA5设置低电平
        PWM_SetCompare3(-Speed);                //PWM设置为负的速度值，此时PWM值为负，但是PWM只能给正数
    }
}