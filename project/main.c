/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */

#include "common.h"
#include "main.h"
#include "init.h"

volatile u8 cycTimer;
u8 g_Timer;

extern int _fctcpy(char name);//将RAM中运行函数代码复制到RAM中，库

main()
{
	_fctcpy('F');
	disable_irq();
	IWDG_500ms();
	CLK_8M_init();
	TIM4_1MS_init();
	delay_ms(20);
	enable_irq();

	while (1){
		if(tryGetInc(g_Timer,cycTimer) >= 10){//10ms
			g_Timer += 10;
		}
	}

}
