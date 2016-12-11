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
#include "queue.h"
#include "semphr.h"

__IO u32 TimingCount=0;
u16 time1, time2;

#define LED2_ON	GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define LED2_OFF	GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define LED1_ON	GPIO_ResetBits(GPIOE, GPIO_Pin_5)
#define LED1_OFF	GPIO_SetBits(GPIOE, GPIO_Pin_5)

#define TaskNum		55
//LCD显示坐标范围 240*320

//void show_dot(u16 x, u16 y);
void my_task(void *arg);
void my_task1();
void led_init(void);
u16 time2_time1(u16 time2, u16 time1);

TaskHandle_t taskHandleSave[TaskNum];
xSemaphoreHandle XinHaoLiang;
xQueueHandle myQueue;

int main()
{
	printf_init(115200);		//使用printf()函数
	SysTick_Config(SystemCoreClock/configTICK_RATE_HZ);		//设置操作系统时钟心跳
	TIM3_tick_Init();

	XinHaoLiang = xSemaphoreCreateMutex();
	xTaskCreate(my_task, "myTask", configMINIMAL_STACK_SIZE, "my_task start... ...\r\n", 2, NULL);
	vTaskStartScheduler();
	while(1)
	{
	}
}

void my_task(void *arg)
{
	u16 i;
	char msg[] = "1234567890";
	char getMsg;
	
	printf(arg);
	led_init();
	myQueue = xQueueCreate(10, sizeof(char));
	for(i=0;i<TaskNum;i++)
	{
		xTaskCreate(my_task1, "myTask1", configMINIMAL_STACK_SIZE, NULL, i+3, &taskHandleSave[i]);
	}
	while(1)
	{
		
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	
// 		//挂起所有任务并记录时间
// 		printf("Suspend Task\r\n");
// 		for(i=0;i<TaskNum;i++)
// 		{
// 			time1 = TIM_GetCounter(TIM3);
// 			vTaskSuspend(taskHandleSave[i]);
// 			time2 = TIM_GetCounter(TIM3);
// 			printf("i=%d, time1=%d, time2=%d, time2-time1=%d\r\n", i, time1, time2, time2-time1);
// 		}
// 		//唤醒所有任务并记录时间
// 		printf("Resume the suspended task ourselves\r\n");
// 		for(i=0;i<TaskNum;i++)
// 		{
// 			time1 = TIM_GetCounter(TIM3);
// 			vTaskResume(taskHandleSave[i]);
// 			time2 = TIM_GetCounter(TIM3);
// 			printf("i=%d, time1=%d, time2=%d, time2-time1=%d\r\n", i, time1, time2, time2-time1);
// 		}
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
		

		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	
// 		//信号量获取并释放 记录时间
// 		time1 = TIM_GetCounter(TIM3);
// 		if(xSemaphoreTake(XinHaoLiang, 50)==pdTRUE)
// 		{
// 			printf("get xinhaoliang ok\r\n");
// 		}else{
// 			printf("get xinhaoliang faile\r\n");
// 		}
// 		time2 = TIM_GetCounter(TIM3);
// 		printf("time1=%d, time2=%d, time2-time1=%d\r\n", time1, time2, time2_time1(time2,time1));
// 		
// 		time1 = TIM_GetCounter(TIM3);
// 		if(xSemaphoreGive(XinHaoLiang)==pdTRUE)
// 		{
// 			printf("Give xinhaoliang ok\r\n");
// 		}else{
// 			printf("Give xinhaoliang faile\r\n");
// 		}
// 		time2 = TIM_GetCounter(TIM3);
// 		printf("time1=%d, time2=%d, time2-time1=%d\r\n", time1, time2, time2_time1(time2,time1));
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	
		//消息队列测试
// 		printf("QueueSend test\r\n");
// 		for(i=0;i<10;i++)
// 		{
// 			time1 = TIM_GetCounter(TIM3);
// 			if(xQueueSend(myQueue, (void*)&msg[i], 20) == pdTRUE)
// 			{
// 				//printf("QueueSend TRUE\r\n");
// 				time2 = TIM_GetCounter(TIM3);
// 				printf("msg=%c, time1=%d, time2=%d, time2-time1=%d\r\n",msg[i], time1, time2, time2_time1(time2,time1));
// 			}
// 		}
// 		printf("QueueReceive test\r\n");
// 		for(i=0;i<10;i++)
// 		{
// 			time1 = TIM_GetCounter(TIM3);
// 			if(xQueueReceive(myQueue, &getMsg, 20)==pdTRUE)
// 			{
// 				//printf("QueueReceive TRUE\r\n");
// 				time2 = TIM_GetCounter(TIM3);
// 				printf("msg=%c, time1=%d, time2=%d, time2-time1=%d\r\n",getMsg, time1, time2, time2_time1(time2,time1));
// 			}
// 		}
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

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
	__IO UBaseType_t myPrio;
	myPrio = uxTaskPriorityGet(NULL);
	while(1)
	{
		vTaskDelay(50);
		//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	
		//测试任务调度上下文切换时间
		myPrio = uxTaskPriorityGet(NULL);
		if(myPrio < TaskNum+2)
		{
			time1 = TIM_GetCounter(TIM3);
			vTaskResume(taskHandleSave[myPrio-1]);
			vTaskSuspend(NULL);
		}else{
			time1 = TIM_GetCounter(TIM3);
			vTaskResume(taskHandleSave[0]);
			vTaskSuspend(NULL);
		}
		time2 = TIM_GetCounter(TIM3);
		printf("time1=%d, time2=%d, time2-time1=%d\r\n", time1, time2, time2_time1(time2,time1));
		//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
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

u16 time2_time1(u16 time2, u16 time1)
{
	if(time2<time1)
	{
		return 0xffff-time1+time2;
	}else{
		return time2-time1;
	}
}
