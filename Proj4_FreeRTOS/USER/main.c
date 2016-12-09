#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "beep.h"
#include "lcd.h"
//FreeRTos 
#include "freertos.h"
#include "task.h"

#define LED2_ON	GPIO_ResetBits(GPIOB, GPIO_Pin_5)
#define LED2_OFF	GPIO_SetBits(GPIOB, GPIO_Pin_5)
#define LED1_ON	GPIO_ResetBits(GPIOE, GPIO_Pin_5)
#define LED1_OFF	GPIO_SetBits(GPIOE, GPIO_Pin_5)

//LCD显示坐标范围 240*320

//void show_dot(u16 x, u16 y);
void my_task(void);
void my_task2();
void led_init(void);

int main()
{
	USART1_Int(9600);		//使用printf()函数
	SysTick_Config(SystemCoreClock/configTICK_RATE_HZ);		//设置操作系统时钟心跳
	
	xTaskCreate(my_task, "myTask", configMINIMAL_STACK_SIZE+50, NULL, 2, NULL);
	xTaskCreate(my_task2, "myTask2", configMINIMAL_STACK_SIZE+50, NULL, 3, NULL);
	vTaskStartScheduler();
	while(1)
	{
	}
}

void my_task()
{
	volatile static u32 time_count;
	led_init();
	while(1)
	{
		LED1_ON;
		LED2_OFF;
		vTaskDelay(50);
		LED1_OFF;
		LED2_ON;
		vTaskDelay(50);
	}
}

void my_task2()
{
	volatile static u32 time_count;
	led_init();
	while(1)
	{
		LED1_ON;
		LED2_OFF;
		vTaskDelay(50);
		LED1_OFF;
		LED2_ON;
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
