#include "BEEP.h"

void BEEP_Init(void)
{
	RCC->AHB1ENR |= 1<<5;
	//Port F, PIN8, �������, ����(δ��ͨ)
	GPIO_Set(GPIOF,PIN8,GPIO_MODE_OUT,GPIO_OTYPE_PP,GPIO_SPEED_100M,GPIO_PUPD_PD);
	BEEP = 0;//��ʼ�رշ�����
}

