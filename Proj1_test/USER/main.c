#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "uart.h"
#include "delay.h"
#include "beep.h"
#include "lcd.h"
//FreeRTos 
#include "freertos.h"
#include "task.h"

__IO u32 TimingCount=0;

#define LED2_ON	GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define LED2_OFF	GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define LED1_ON	GPIO_ResetBits(GPIOE, GPIO_Pin_5)
#define LED1_OFF	GPIO_SetBits(GPIOE, GPIO_Pin_5)

#define TaskNum		2
//LCD显示坐标范围 240*320

//void show_dot(u16 x, u16 y);
void my_task(void *arg);
void my_task1();
void led_init(void);

TaskHandle_t taskHandleSave[TaskNum];

int main()
{
	u16 i;
	printf_init(115200);		//使用printf()函数
	SysTick_Config(SystemCoreClock/configTICK_RATE_HZ);		//设置操作系统时钟心跳
	TIM3_tick_Init();

	xTaskCreate(my_task, "myTask", configMINIMAL_STACK_SIZE, "my_task start... ...\r\n", 2, NULL);
	for(i=0;i<TaskNum;i++)
	{
		xTaskCreate(my_task1, "myTask1", configMINIMAL_STACK_SIZE, NULL, i+3, &taskHandleSave[i]);
	}
	vTaskStartScheduler();
	while(1)
	{
	}
}

void my_task(void *arg)
{
	u16 i;
	u16 time1, time2;
	printf(arg);
	led_init();
	while(1)
	{
		printf("Suspend Task\r\n");
		for(i=0;i<TaskNum;i++)
		{
			time1 = TIM_GetCounter(TIM3);
			vTaskSuspend(taskHandleSave[i]);
			time2 = TIM_GetCounter(TIM3);
			printf("i=%d, time1=%d, time2=%d, time2-time1=%d\r\n", i, time1, time2, time2-time1);
		}
		
		printf("Resume the suspended task ourselves\r\n");
		for(i=0;i<TaskNum;i++)
		{
			time1 = TIM_GetCounter(TIM3);
			vTaskResume(taskHandleSave[i]);
			time2 = TIM_GetCounter(TIM3);
			printf("i=%d, time1=%d, time2=%d, time2-time1=%d\r\n", i, time1, time2, time2-time1);
		}
		
		LED1_ON;
		LED2_OFF;
		vTaskDelay(50);
		LED1_OFF;
		LED2_ON;
		vTaskDelay(50);
		
	}
}

void my_task1()
{
	__IO UBaseType_t temp;
	temp = uxTaskPriorityGet(NULL);
	while(1)
	{
		vTaskDelay(50);
	}
}

void led_init()
{
	GPIO_InitTypeDef gpin_init_struct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);
	gpin_init_struct.GPIO_Pin = GPIO_Pin_5;
	gpin_init_struct.GPIO_Speed = GPIO_Speed_2MHz;
	gpin_init_struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB, &gpin_init_struct);				//B?led1 
	GPIO_Init(GPIOE, &gpin_init_struct);				//E?led2 
}

/*
void show_dot(u16 x, u16 y)
{
	if(x>310||y>230) return;
	LCD_Fill(x,y,x+10,y+10,GREEN);
	Delay_ms(200);
	LCD_Fill(x,y,x+10,y+10,BLACK);
}
*/
