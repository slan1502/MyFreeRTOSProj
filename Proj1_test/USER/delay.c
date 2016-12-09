/*-------------------------------------------------------------------------------
�ļ����ƣ�SysTick.c
�ļ�������SysTick ϵͳ�δ�ʱ��1us�жϺ�����,�ж�ʱ����������ã�
          ���õ��� 1us 10us 1ms �жϡ�        
��    ע������Ĭ��ʹ��72Mʱ�ӣ��޷�Ƶ 
---------------------------------------------------------------------------------*/
#include "delay.h"

static __IO u32 TimingDelay;

/*-------------------------------------------------------------------------------
�������ƣ�SysTick_Init
��������������ϵͳ�δ�ʱ�� SysTick
�����������
���ز�������
��    ע��ʹ�ô˹���֮ǰ��Ҫ���ô˺�������ʼ���Ĵ����Ȳ���
---------------------------------------------------------------------------------*/
void SysTick_Init(void)
{
	 // SystemFrequency / 1000     1ms�ж�һ��
	 // SystemFrequency / 100000	 10us�ж�һ��
	 // SystemFrequency / 1000000  1us�ж�һ��
	 
	if (SysTick_Config(SystemCoreClock / 1000000))	// 1us ST3.5.0��汾
	{ 
		/* Capture error */ 
		while (1);
	}
}

//��ʱ���ж�Ƕ�׿���
void NVIC_cfg()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//ѡ���жϷ���2
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;		//ѡ��ʱ��TIM2���ж�ͨ��      
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//��ռʽ�ж����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//��Ӧʽ�ж����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);
}
/*-------------------------------------------------------------------------------
�������ƣ�TIM2_tick_Init
��������������ϵͳTIM2��ʱ�� ��ʱʱ��5us
�����������
���ز�������
��    ע��ʹ�ô˹���֮ǰ��Ҫ���ô˺�������ʼ���Ĵ����Ȳ���
---------------------------------------------------------------------------------*/
void TIM2_tick_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_cfg();		//�ж�Ƕ�����ó�ʼ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);		//������ʱ��TIM2������ʱ��
	TIM_DeInit(TIM2);								//����TIM2��ʼ��ΪĬ��ֵ
	TIM_InternalClockConfig(TIM2);	//���ڲ�ʱ�ӽoTIM2�ṩʱ��Դ
	TIM_TimeBaseStructure.TIM_Prescaler = 9;		//����Ƶ�ʲ���Ƶ ʹ��72MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//TIM2ʱ�ӷ�Ƶģʽ ����Ϊ ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//������ʽ
	TIM_TimeBaseStructure.TIM_Period = 35;			//��ʱ�������    
	//��ʱʱ����㹫ʽ
	//T = (TIM_Period+1)*(TIM_Prescaler+1)/72MHz
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);				//�����־λ
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);		//ʹ�ܶ�ʱ��TIM2�ж�
	TIM_Cmd(TIM2, ENABLE);											//ʹ�ܶ�ʱ��
}

/*-------------------------------------------------------------------------------
�������ƣ�TIM3_tick_Init
��������������ϵͳTIM3��ʱ�� ��ʱƵ��Ϊ72MHz �������ж�
�����������
���ز�������
��    ע��ʹ�ô˹�������һ����ʱ����ͨ������TIM_GetCounter(TIM3)������ѯʱ�䳣��					
---------------------------------------------------------------------------------*/
void TIM3_tick_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);		//������ʱ��TIM3������ʱ��
	TIM_DeInit(TIM3);								//����TIM3��ʼ��ΪĬ��ֵ
	TIM_InternalClockConfig(TIM3);	//���ڲ�ʱ�ӽoTIM3�ṩʱ��Դ
	TIM_TimeBaseStructure.TIM_Prescaler = 0;		//����Ƶ�ʲ���Ƶ ʹ��72MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//TIM3ʱ�ӷ�Ƶģʽ ����Ϊ ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//������ʽ
	TIM_TimeBaseStructure.TIM_Period = 0xffff;			//��ʱ�������    
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);				//�����־λ
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);		//ʹ�ܶ�ʱ��TIM2�ж�
	TIM_Cmd(TIM3, ENABLE);											//ʹ�ܶ�ʱ��
}

/*-------------------------------------------------------------------------------
�������ƣ�Delay_us
������������ʱnTime 1us  
���������nTime
���ز�������
��    ע����
---------------------------------------------------------------------------------*/
void Delay_5us(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}

/*-------------------------------------------------------------------------------
�������ƣ�TimingDelay_Decrement
�������������ֵݼ�ֱ��0  
�����������
���ز�������
��    ע����Ҫ���жϺ���SysTick_Handler()�е���
---------------------------------------------------------------------------------*/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
	TimingDelay--;
	}
}
/*----------------------�·��� ������̳��www.doflye.net--------------------------*/

/*��ʱ��TIM2�жϴ�����*/
void TIM2_IRQHandler(void)
{
	//����Ƿ����ж����
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
		TimingDelay_Decrement();
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);				//�����־λ
	}
}

