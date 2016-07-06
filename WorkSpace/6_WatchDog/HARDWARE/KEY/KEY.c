#include "KEY.h"

//��4������,3����PE��(�͵�ƽ��Ч,����), 1����PA��(�ߵ�ƽ��Ч,����)
void KEY_Init(void)
{
	RCC->AHB1ENR = 1<<4;
	RCC->AHB1ENR = 1<<0;
	GPIO_Set(GPIOE,PIN3|PIN4|PIN2,GPIO_MODE_IN,0,0,GPIO_PUPD_PU);//��������
	GPIO_Set(GPIOE,PIN0,GPIO_MODE_IN,0,0,GPIO_PUPD_PD);//����ʱMODE��SPEED��������
	
}

//����������
//���ذ���ֵ
//mode:0,��֧��������;1,֧��������;
//0��û���κΰ�������
//1�� KEY0 ����
//2�� KEY1 ����
//3�� KEY2 ����
//4�� KEY_UP ���� �� WK_UP
//ע��˺�������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY_UP!!


u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־
	if(mode)
		key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||WK_UP==1))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)return 1;
		else if(KEY1==0)return 2;
		else if(KEY2==0)return 3;
		else if(WK_UP==1)return 4;
	}
	else if(KEY0==1&&KEY1==1&&KEY2==1&&WK_UP==0)
			key_up=1; 	    
 	return 0;// �ް�������
}

//�Զ��尴��ɨ�躯��
//seccessional_press 0: ֧������(����һֱ����һ������)
//seccessional_press 1: ��֧������(�����־Ͳ����½���)
//����ֵ: ����modeֵ


u8 KEY_Scan_Custom(u8 successional_press)
{
	switch(successional_press)
	{
		case 1:
		if(KEY0==0||KEY1==0||KEY2==0||WK_UP==1)//����κ�һ������������
		{
				delay_ms(30);//����
				if(KEY0==0) 
				{
					while(KEY0==0);//�ȴ������ͷ�
					delay_ms(10);
					return 1;//����ģʽ1
				}
				else if(KEY1==0) 
				{
					while(KEY1==0);
					delay_ms(10);
					return 2;//����ģʽ1
				}
				else if(KEY2==0) 
				{
					while(KEY2==0);
					delay_ms(10);
					return 3;//����ģʽ1
				}
				else if(WK_UP==1) 
				{
					while(WK_UP==1);
					delay_ms(10);
					return 4;//����ģʽ1
				}
				return 0;
			}

		case 0://֧������
		if(KEY0==0||KEY1==0||KEY2==0||WK_UP==1)
		{
			delay_ms(30);
			if(KEY0==0)
			{
				delay_ms(100); 
				return 1;	//�ȴ��̵ܶ�һ˲�� ���һֱ����ĳ���� �ͻ᲻�Ϸ������ֵ
			}
			else if(KEY1==0)
			{
				delay_ms(100); 
				return 2;
			}
			else if(KEY2==0)
			{
				delay_ms(100); 
				return 3;
			}
			else if(WK_UP==1)
			{
				delay_ms(100); 
				return 4;
			}
			else return 0;

		}


	}



	
}



