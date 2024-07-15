#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

/*
*   函   数：  串口初始化
*   参   数：  无
*   返回 值：   无
*/

void Serial_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*USART初始化*/
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;                                  //波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //硬件流控制，不需要
    USART_InitStructure.USART_Mode = USART_Mode_Tx;             //模式，选择发送模式
    USART_InitStructure.USART_Parity = USART_Parity_No;         //奇偶校验， 不需要
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //停止位， 选择1位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b; //字长， 选择8位
    USART_Init(USART1, &USART_InitStructure);
    
    /*USART使能*/
    USART_Cmd(USART1, ENABLE);
}

/*
*   函   数：  串口发送一个字节
*/
void Serial_SendByte(uint8_t Byte) 
{
    USART_SendData(USART1, Byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);   //等待发送完成
    /*下次写入数据寄存器时会自动清除发送完成标志位，故此循环后，无需清除标准位*/
}

/*
*   函   数：  串口发送一个数组
*   参   数：  Array要发送的数组的首地址
*   参   数：  Length要发送的数组的长度
*   返回 值：  无
*/
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
    uint16_t i;
    for (i = 0; i < Length; i++)        //遍历数组
    {
        Serial_SendByte(Array[i]);      //依次调用Serial——SendByte发送每个字节数据
    }
}

/*
*   函   数：  串口发送一个字符串
*   参   数：  String要发送的数组的首地址
*   返回 值：  无
*/
void Serial_SendString(char *String)
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++)         //遍历字符串数组，遇到字符串结束标志后停止
    {
        Serial_SendByte(String[i]);             //调用Serial——SendByte发送每个字节数据
    }
}

/*
*   函   数：  次方函数
*   返回 值：  等于X的Y次方
*/
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
    uint32_t Result = 1;
    while (Y--)
    {
        Result *= X;
    }
    
    return Result;
}

/*
*   函   数：  串口发送数字
*   参   数：  NUmber要发送的数字
*   参   数：  Length要发送的数组的长度，范围0-10
*   返回 值：  无
*/
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
    uint8_t i;
    for (i=0; i < Length; i++)
    {
        Serial_SendByte(Number / Serial_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/*
*   函   数：  使用printf函数需要重定向底层函数
*   参   数：  保持原格式即可，无需变动
*   返回 值：  保持原格式即可，无需变动
*/
int fputc(int ch, FILE *f)
{
    Serial_SendByte(ch);        //将printf的底层重定向到自己的发送字节函数
    return ch;
}

/*
*   函   数：  自己封装printf函数
*   参   数：  format格式化字符串
*   参   数：  可变的参数列表
*   返回 值：  无
*/
void Serial_Printf(char *format, ...)
{
    char String[100];
    va_list arg;
    va_start(arg, format);
    vsprintf(String, format, arg);
    va_end(arg);
    Serial_SendString(String);
}
