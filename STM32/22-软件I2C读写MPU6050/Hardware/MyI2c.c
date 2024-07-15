#include "stm32f10x.h"                  // Device header
#include "Delay.h"

/*引脚配置*/

/*
*   函   数：  I2C写SCL引脚电平
*   参   数：  BitValue协议层传入的当前需要写入的SCL电平，范围0-1
*   返回值 ：   无
*   注意事项：   此函数需要用户实现内容， 当BitValue为0时， 需要SCL为低电平，当BitValue为1时， 需要SCL为高电平
*/

void MyI2C_W_SCL(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_10, (BitAction)BitValue);
    Delay_us(10);
}

/*
*   函   数：  I2C写SDA引脚电平
*   参   数：  BitValue协议层传入的当前需要写入的SDA电平，范围0-0xFF
*   返回值 ：   无
*   注意事项：   此函数需要用户实现内容， 当BitValue为0时， 需要SDA为低电平，当BitValue非0时， 需要SDA为高电平
*/
void MyI2C_W_SDA(uint8_t BitValue)
{
    GPIO_WriteBit(GPIOB, GPIO_Pin_11, (BitAction)BitValue);
    Delay_us(10);
}

/*
*   函   数：  I2C读SDA引脚电平
*   参   数：  无
*   返回值 ：  协议层需要得到当前SDA的电平，范围0-1
*   注意事项： 此函数需要用户实现内容，SDA为低电平，返回0；当SDA为高电平，返回1
*/
uint8_t MyI2C_R_SDA(void)
{
    uint8_t BitValue;
    BitValue = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    Delay_us(10);
    return BitValue;
}

/*
*   函   数：  I2C初始化
*   参   数：  无
*   返回值：    无
*   注意事项： 此函数需要用户实现内容，实现SCL、SDA引脚的初始化
*/
void MyI2C_Init(void)
{
    /*开启时钟*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    /*GPIO初始化*/
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    /*设置默认电平*/
    GPIO_SetBits(GPIOB, GPIO_Pin_10 | GPIO_Pin_11);     //将引脚PB10和PB11引脚初始化后默认设置为高电平
}

/*协议层*/

/*
*   函   数：I2C起始
*   参   数：无
*   返回值 ：无
*   描   述：SCL高电平期间，SDA从高到低，产生起始信号，随后SCL也从高到低
*/

void MyI2C_Start(void)
{
    MyI2C_W_SDA(1);         //确保SDA高电平
    MyI2C_W_SCL(1);         //确保SCL高电平
    MyI2C_W_SDA(0);         //拉低SDA，产生起始信号
    MyI2C_W_SCL(0);         //SCL也拉低，占用总线
}

/*
*   函   数：I2C终止
*   参   数：无
*   返回值 ：无
*   描   述：终止条件： SCL 高电平期间， SDA 从低电平切换到高电平
*/
void MyI2C_Stop(void)
{
    MyI2C_W_SDA(0);         //确保SDA低电平
    MyI2C_W_SCL(1);         //确保SCL高电平
    MyI2C_W_SDA(1); 
}

/*
*   函   数：  I2C发送一个字节
*   参   数：  Byte要发送的一个字节，范围0x00-0xFF
*   返回  值： 无
*   描   述：  发送一个字节： SCL 低电平期间，主机将数据位依次放到 SDA 线上（高位先行），
*               然后释放 SCL ，从机将在 SCL 高电平期间读取数据位，
*/
void MyI2C_SendByte(uint8_t Byte)
{
    //进来之后SCL是低电平
    uint8_t i;
    for (i = 0; i < 8; i++)         //循环8次，主机依次发送数据的每一位
    {
        MyI2C_W_SDA(Byte & (0x80 >> i));        //使用掩码的方式取高位数据写入SDA
        MyI2C_W_SCL(1);                         // 释放低SCL
        MyI2C_W_SCL(0);                         //拉低SCL为发送下一位数据准备
    }
}

/*
*   函   数：  I2C接收一个字节
*   参   数：  Byte要接收一个字节，范围0x00-0xFF
*   返回  值： 无
*   描   述： 主机将在 SCL 高电平期间读取数据位，
*/
uint8_t MyI2C_ReciveByte(void)
{
    //进来之后SCL是低电平
    uint8_t i, Byte = 0x00;
    MyI2C_W_SDA(1);             //主机释放SDA，同时从机往SDA上放数据
    for (i = 0; i < 8; i++)
    {
        MyI2C_W_SCL(1);  //释放SCL, 主机将在SCL高电平期间读取数据位
        if (MyI2C_R_SDA() == 1)
        {
            Byte |= (0x80 >> i);
        }
        MyI2C_W_SCL(0);  
    }
    return Byte;
}

/*
*   函   数：  I2C发送应答信号
*   参   数：  Byte要发送的应答位，范围0-1， 0 表示应答， 1表示非应答
*   返回  值： 无
*   描   述：  发送应答：主机在接收完一个字节之后，在下一个时钟发送一位数
*              据，数据 0 表示应答，数据 1 表示非应答
*/
void MyI2C_SendAck(uint8_t AckBite)
{
    MyI2C_W_SDA(AckBite);               //主机把应答信号放到SDA线
    MyI2C_W_SCL(1);                     //释放SCL,从机在SCL高电平期间读取应答信号
    MyI2C_W_SCL(0);                     //拉低SCL，为下一个阶段做准备
}

/*
*   函   数：  I2C接收应答信号
*   参   数：  无
*   返回  值： 接收到的应答信号，0 表示应答，数据 1 表示非应答
*   描   述：  主机在接收之前，需要释放 SDA
*/
uint8_t MyI2C_ReceiveAck(void)
{
    uint8_t AckBit;
    MyI2C_W_SDA(1);             //接收信号前确保释放SDA，
    MyI2C_W_SCL(1);             //释放SCL，主机在SCL高电平期间读取SDA
    AckBit = MyI2C_R_SDA();     //将应答信号存储在变量里
    MyI2C_W_SCL(0);             //拉低SCL， 开始下一个时序
    return AckBit;
}