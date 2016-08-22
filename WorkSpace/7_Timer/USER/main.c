#include "sys.h"
#include "delay.h"

#include "led.h"
#include "timer.h"


int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����Ϊ2 2λ��ռ���ȼ�
	delay_init(168);
	LED_Init();		//��ʼ��LED�˿�
	TIM3_Int_Init(5000-1, 8400-1);	//��ʱ��ʱ��84M, ��Ƶϵ��8400, ����84M/8400 = 10KHz,����5000��Ϊ500ms

	while(1)
	{
		LED0 = !LED0;
		delay_ms(200);	//LED0ÿ��200ms��תһ��, ͬʱLED1�ڶ�ʱ��3ÿ���һ�ξͷ�תһ��
	}
}	
