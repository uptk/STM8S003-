#include <common.h>

#define	nop4()		_asm("nop");_asm("nop");_asm("nop");_asm("nop");
void ADC_init(void)                                 
{
  ADC_CR1 = 0;//fadc = fmaster/2 = 8M,single mode,off                     
  ADC_CR2 = (1<<3)|(0<<1);//right align                        
  
  ADC_CSR = 0;                         
// ADC_TDRL = (1<<6)|(1<<3);                                     
}

u16 ADC_get_val(u8 ch)
{
	u16 sum,min = 0xffff,max = 0,cnt;
	u8 try_cnt;
	ADC_CSR = (ADC_CSR & 0x70)|ch;
	ADC_CR1 |= 1;
	nop4();//32 cycle for establish
	nop4();
	nop4();
	nop4();
	nop4();
	nop4();
	nop4();
	nop4();
	
	for(cnt = 0,sum = 0;cnt < 10;cnt++){
		u16 tmp;
		ADC_CR1 |= 1;
		#ifndef	DEBUG
		while(!(ADC_CSR & 0x80));
		#else
		try_cnt = 100;
		while((!(ADC_CSR & 0x80))&&(try_cnt--));
		if(!try_cnt){
			//BUG_ON();
			if(!(error.flag & ERR_ADC)){
				error.flag |= ERR_ADC;
			}
		}
		#endif
		ADC_CSR &= 0x7f;
		tmp = ADC_DRL;
		tmp |= (ADC_DRH&3)<<8;
		sum += tmp;
		if(min > tmp)
			min = tmp;
		if(max < tmp)
			max = tmp;
	}
	ADC_CR1 &= ~1;
	return (sum - min - max)/8;
}