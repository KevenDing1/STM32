#include "EXTI.h"
#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h"
#include "beep.h"

void EXTI0_IRQHandler(void)
{
	delay_ms(10);//����
	if(WK_UP == 1)
		BEEP = !BEEP;
	EXTI->PR = 1<<0;	//�����Ӧ�����жϱ�־
}

void EXTI1_IRQHandler(void)
{
	delay_ms(10);
	if(KEY2 == 0)
		LED0 = !LED0;
	EXTI->PR = 1<<2;//KEY2��ӦPE2
}

void EXTI2_IRQHandler(void)
{
	delay_ms(10);
	if(KEY1 == 0)
		LED1 = !LED1;
	EXTI->PR = 1<<3;//KEY3��ӦPE3
}

void EXTI3_IRQHandler(void)
{
	delay_ms(10);//����
	if(KEY0 == 0)
	{
		LED0 = !LED0;
		LED1 = !LED1;
	}
	EXTI->PR = 1<<4;
}


void EXTIX_Init(void)
{
	KEY_Init();
	
	//���ü����жϴ�����ʽ
	Ex_NVIC_Config(GPIO_E,2,FTIR);
	Ex_NVIC_Config(GPIO_E,3,FTIR);
	Ex_NVIC_Config(GPIO_E,4,FTIR);//����,Ĭ�ϸߵ�ƽ,�½��ش���
	Ex_NVIC_Config(GPIO_A,0,RTIR);//����,Ĭ�ϵ͵�ƽ,�����ش���
	
	//�����жϷ���
	MY_NVIC_Init(3,2,EXTI2_IRQn,2);//��ռ���ȼ�Ϊ3,�����ȼ�Ϊ2,�жϷ���Ϊ��2��[2λ��ռ���ȼ�(��4����ռ���ȼ�),2λ��Ӧ���ȼ�(��4��)]
	MY_NVIC_Init(2,2,EXTI3_IRQn,2);
	MY_NVIC_Init(1,2,EXTI4_IRQn,2);
	MY_NVIC_Init(0,2,EXTI0_IRQn,2);	
	
}



