#include "common.h"
#include "setIrqPriority.h"

/*
I1 I0 Priority Level
1 0 Level 0 (main)
Low
High
0 1 Level 1
0 0 Level 2
1 1 Level 3 (= software priority disabled*)
*/
void setIrqPriority(u8 irq,u8 level)
{
	static u8 const levelCode[4] = {2,1,0,3};
	if((irq > 0)&&(irq <= 29)){
		volatile u8 *regStart = &ITC_SPR1;
		u8 tmp;
		u8 i = irq>>2;
		u8 j = (irq&3)*2;
		if(level >= 4)
			level = 3;
		tmp = regStart[i];
		tmp &= ~(3<<j);
		tmp |= levelCode[level]<<j;
		regStart[i] = tmp;
	}
}