#include "sys.h"
#include "delay.h"
#include "LED.h"
#include "KEY.h"
#include "BEEP.h"
#include "EXTI.h"
#include "usart.h"
int main(void)
{
	Stm32_Clock_Init(336,8,2,7);
	delay_init(168);
	uart_init(84,115200);//��ʼ�����ں����ֱ����printf�ڴ��ڴ�ӡ��Ϣ
	LED_Init();
	KEY_Init();
	BEEP_Init();
	EXTIX_Init();
	LED0 = 0; 	//�������
	
	while(1)
	{
		printf("OK\r\n");
		delay_ms(500);
	}
	
}
