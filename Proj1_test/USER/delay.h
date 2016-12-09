#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f10x.h"

void SysTick_Init(void);
void TIM2_tick_Init(void);
void TIM3_tick_Init(void);
void Delay_5us(__IO u32 nTime);	  //单位1us

#define Delay_ms(x) Delay_5us(200*x)//单位ms

#endif 
/*----------------------德飞莱 技术论坛：www.doflye.net--------------------------*/
