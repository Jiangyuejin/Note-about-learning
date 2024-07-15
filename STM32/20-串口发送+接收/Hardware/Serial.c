#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_RxData;              //定义串口接收的数据变量
uint8_t Serial_RxFlag;              //定义串口接收的标志位变量

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
    GPIO_Init(GPIOA, &GPIO_InitStructure);                       //将PA9复用为推挽输出
    
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;         
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      
    GPIO_Init(GPIOA, &GPIO_InitStructure);                       //将PA10引脚初始化为上拉输入
    
    /*USART初始化*/
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;                       //波特率
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制，不需要           
    USART_InitStructure.USART_Mode = USART_Mode_Rx  | USART_Mode_Tx;    //模式，发送模式和接收模式均选择
    USART_InitStructure.USART_Parity = USART_Parity_No;                 //奇偶校验， 不需要
    USART_InitStructure.USART_StopBits = USART_StopBits_1;              //停止位， 1位
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;         //字长， 选择8位， 如果选择奇偶校验，字长9位
    USART_Init(USART1, &USART_InitStructure);
    
    /*中断输出配置*/
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);                      //接收数据寄存器非空标志位
    
    /*NVIC中断分组*/
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    
    /*NVIC配置*/
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_Init(&NVIC_InitStructure);
    
    /*USRAT使能*/
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

/*
*   函   数：  获取串口接收标志
*   参   数：  无
*   返回 值：  串口接收标志， 范围0-1，接收到数据后，标志位置1，读取标志位后自动清零
*/
uint8_t Serial_GetRxFlag(void)
{
    if (Serial_RxFlag == 1)         //如果标志位为1
    {
        Serial_RxFlag = 0;
        return 1;                   //则返回1， 并自动清除零标志位
    }
    return 0;
}

/*
*   函   数：  获取串口接收的数据
*   参   数：  无
*   返回 值：  接收数据，范围0-255
*/
uint8_t Serial_GetRxData(void)
{
    return Serial_RxData;       //返回接收的数据变量
}

/*
*   函   数:  USART1_IRQHandler
*   参   数:  无
*   返回值 无
*   注意事项：   此函数为中断函数，无需调用
*/

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)        //判断是否是USART1的接收事件触发中断
    {
        Serial_RxData = USART_ReceiveData(USART1);              //读取数据寄存器，存放在接收的数据变量
        Serial_RxFlag   = 1;                                    //置接收标志位变量为1
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);         //清除USART1的RXEN标志位
                                                                //读取清除此标志位
    }
}