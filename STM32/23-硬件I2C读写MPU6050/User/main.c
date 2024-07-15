#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"

uint8_t ID;
int16_t AX, AY, AZ, GX, GY, GZ;

int main (void)
{
    /*模块初始化*/
    OLED_Init();
    MPU6050_Init();
    
    /*OLED显示*/
    OLED_ShowString(1, 1, "ID:"); //  1行3列显示字符串
    ID = MPU6050_GetID();
    OLED_ShowHexNum(1, 4, ID, 2);
    
    while (1)
    {
        MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);
        OLED_ShowSignedNum(2, 1, AX, 5);
        OLED_ShowSignedNum(3, 1, AY, 5);
        OLED_ShowSignedNum(4, 1, AZ, 5);
        OLED_ShowSignedNum(2, 8, GX, 5);
        OLED_ShowSignedNum(3, 8, GY, 5);
        OLED_ShowSignedNum(4, 8, GZ, 5);
    }
}