#include "LED.h"

void LED_Init(void)
{
	RCC->AHB1ENR |= 1<<5;	//ʹ��Port Fʱ��
	GPIO_Set(GPIOF,PIN9|PIN10,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M, GPIO_PUPD_PU);
	LED0 = 1;
	LED1 = 1;//��ʼ������Ϩ��(�ߵ�ƽ��ֹ,�͵�ƽ��ͨ)
}
















