/*-------------------------------------------------------------------------------
文件名称：SysTick.c
文件描述：SysTick 系统滴答时钟1us中断函数库,中断时间可自由配置，
          常用的有 1us 10us 1ms 中断。        
备    注：程序默认使用72M时钟，无分频 
---------------------------------------------------------------------------------*/
#include "delay.h"

static __IO u32 TimingDelay;

/*-------------------------------------------------------------------------------
程序名称：SysTick_Init
程序描述：启动系统滴答定时器 SysTick
输入参数：无
返回参数：无
备    注：使用此功能之前需要调用此函数，初始化寄存器等参数
---------------------------------------------------------------------------------*/
void SysTick_Init(void)
{
	 // SystemFrequency / 1000     1ms中断一次
	 // SystemFrequency / 100000	 10us中断一次
	 // SystemFrequency / 1000000  1us中断一次
	 
	if (SysTick_Config(SystemCoreClock / 1000000))	// 1us ST3.5.0库版本
	{ 
		/* Capture error */ 
		while (1);
	}
}

//定时器中断嵌套控制
void NVIC_cfg()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//选择中断分组2
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;		//选择定时器TIM2的中断通道      
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//抢占式中断优先级为0
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			//响应式中断优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//使能中断
	NVIC_Init(&NVIC_InitStructure);
}
/*-------------------------------------------------------------------------------
程序名称：TIM2_tick_Init
程序描述：启动系统TIM2定时器 定时时间5us
输入参数：无
返回参数：无
备    注：使用此功能之前需要调用此函数，初始化寄存器等参数
---------------------------------------------------------------------------------*/
void TIM2_tick_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_cfg();		//中断嵌套设置初始化
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);		//启动定时器TIM2的外设时钟
	TIM_DeInit(TIM2);								//设置TIM2初始化为默认值
	TIM_InternalClockConfig(TIM2);	//用内部时钟給TIM2提供时钟源
	TIM_TimeBaseStructure.TIM_Prescaler = 9;		//总线频率不分频 使用72MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//TIM2时钟分频模式 设置为 不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数方式
	TIM_TimeBaseStructure.TIM_Period = 35;			//定时器溢出数    
	//定时时间计算公式
	//T = (TIM_Period+1)*(TIM_Prescaler+1)/72MHz
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);				//清除标志位
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);		//使能定时器TIM2中断
	TIM_Cmd(TIM2, ENABLE);											//使能定时器
}

/*-------------------------------------------------------------------------------
程序名称：TIM3_tick_Init
程序描述：启动系统TIM3计时器 计时频率为72MHz 不开启中断
输入参数：无
返回参数：无
备    注：使用此功能启动一个计时器，通过调用TIM_GetCounter(TIM3)函数查询时间常数					
---------------------------------------------------------------------------------*/
void TIM3_tick_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);		//启动定时器TIM3的外设时钟
	TIM_DeInit(TIM3);								//设置TIM3初始化为默认值
	TIM_InternalClockConfig(TIM3);	//用内部时钟給TIM3提供时钟源
	TIM_TimeBaseStructure.TIM_Prescaler = 0;		//总线频率不分频 使用72MHz
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//TIM3时钟分频模式 设置为 不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//计数方式
	TIM_TimeBaseStructure.TIM_Period = 0xffff;			//定时器溢出数    
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);				//清除标志位
	TIM_ITConfig(TIM3,TIM_IT_Update,DISABLE);		//使能定时器TIM2中断
	TIM_Cmd(TIM3, ENABLE);											//使能定时器
}

/*-------------------------------------------------------------------------------
程序名称：Delay_us
程序描述：延时nTime 1us  
输入参数：nTime
返回参数：无
备    注：无
---------------------------------------------------------------------------------*/
void Delay_5us(__IO u32 nTime)
{ 
	TimingDelay = nTime;	

	while(TimingDelay != 0);
}

/*-------------------------------------------------------------------------------
程序名称：TimingDelay_Decrement
程序描述：数字递减直到0  
输入参数：无
返回参数：无
备    注：需要在中断函数SysTick_Handler()中调用
---------------------------------------------------------------------------------*/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
	TimingDelay--;
	}
}
/*----------------------德飞莱 技术论坛：www.doflye.net--------------------------*/

/*定时器TIM2中断处理函数*/
void TIM2_IRQHandler(void)
{
	//检查是否有中断溢出
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
  {
		TimingDelay_Decrement();
		TIM_ClearFlag(TIM2, TIM_FLAG_Update);				//清除标志位
	}
}

