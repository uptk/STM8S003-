#include "common.h"
#include "init.h"

void CLK_8M_init(void)
{
    CLK_DeInit();
    CLK_HSICmd(ENABLE);//开始内部高频RC
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV2);//HSI为系统时钟
    CLK_SYSCLKConfig(CLK_PRESCALER_CPUDIV1);//不分频,8M
	
}
void TIM4_1MS_init(void)
{
    TIM4_DeInit();
    TIM4_TimeBaseInit(TIM4_PRESCALER_32, 250);//8分频,1MS
    TIM4_Cmd(ENABLE);//使能计时器  
    TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);//使能TIM2中断 
}

void IWDG_500ms(void)                                                                //独立看门狗初始化操作
{  
	IWDG_Enable();
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_256);
	IWDG_SetReload(0xfa);
	IWDG_ReloadCounter();

}

void Uart2_Init(void)
{
	UART2_DeInit();
	UART2_Init(9600, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
	UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);
	UART2_Cmd(ENABLE);
}

void Uart2_SendByte(u8 data)
{
	UART2_SendData8(data);
	
	while (UART2_GetFlagStatus(UART2_FLAG_TXE) == RESET);
}
void Uart2_SendStr(u8 *buf,u8 len)
{
	while(len>0){
		Uart2_SendByte(*buf);
		buf++;
		len--;
	}
}


volatile bool irq_enable;


void ENTER_CRITIAL(void)
{
	if(irq_enable){
		disableInterrupts();
	}
}

void EXIT_CRITIAL(void)
{
	if(irq_enable){
		enableInterrupts();
	}
}

void enable_irq(void)
{
	if(!irq_enable){
		enableInterrupts();
		irq_enable=1;
	}
}

void disable_irq(void)
{
	if(irq_enable){
		disableInterrupts();
		irq_enable=0;
	}
}


/*	记录	

存储								0					1				3				4			...				RecordLen-1
KeyAddr+0							RecordKey+0		~(RecordKey+0)		D00				D01			...				D0(RecordLen-3)
KeyAddr+RecordLenB					RecordKey+1		~(RecordKey+1)		D10				D11			...				D1(RecordLen-3)
KeyAddr+2*RecordLenB				RecordKey+2		~(RecordKey+2)		D20				D21			...				D2(RecordLen-3)
...									...
...									...
KeyAddr+PageSize-RecordLenB			...
*/



u8 IAP_ErasePage(u32 addr)
{
	FLASH_EraseBlock(addr,FLASH_MEMTYPE_DATA);
	return (FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA)!=FLASH_STATUS_SUCCESSFUL_OPERATION);
}
u8 IAP_ProgramUnit(u32 addr,PROG_t dat)
{
	FLASH_ProgramByte(addr,dat);
	return (FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA)!=FLASH_STATUS_SUCCESSFUL_OPERATION);
}


u8 record_no;
NEAR PROG_t record_buf[RecordLen-2];

void get_record(void)
{
	u8 i=0;
	PROG_t *addr=(PROG_t *)KeyAddr;
	while((i<RecordN)&&(addr[0]==(RecordKey+i))&&((addr[0]^addr[1])==KeyCS)){
		i++;
		addr+=RecordLen;
	}

	if((i>=RecordN)||(addr[0]==EraseFill)){
		record_no=i;
		if(i>0){
			u8 j=0;
			addr=addr-RecordLen+2;
			while(j<arraySizeOf(record_buf)){
				record_buf[j]=addr[j];
				j++;
			}
		}
	}else{
		record_no=RecordN+1;
	}
}
u8 write_record_buf(void)
{
	u8 i;
	u8 ret=0;
	PROG_t *start_addr=(PROG_t *)KeyAddr+record_no*RecordLen;
	PROG_t key1 = RecordKey+record_no;
	PROG_t key2 =~key1;
	ret=IAP_ProgramUnit((u32)start_addr,key1);
	if(ret){
		return ret;
	}
	i=2;
	while((i<2+arraySizeOf(record_buf))&&!(ret=IAP_ProgramUnit((u32)(start_addr+i),record_buf[i-2]))){
		i++;
	}
	if(ret){
		return ret;
	}
	ret=IAP_ProgramUnit((u32)(start_addr+1),key2);
	return ret;
}

void reset_record(void)
{
	u8 ret=0;
	get_record();
	if((record_no>1)&&(record_no<=RecordN)){
		ENTER_CRITIAL();
		APROM_Unlock();
		ret=IAP_ErasePage(KeyAddr);
		APROM_Lock();
		EXIT_CRITIAL();
		if(!ret){
			record_no=0;
			ENTER_CRITIAL();
			APROM_Unlock();
			ret=write_record_buf();
			APROM_Lock();
			EXIT_CRITIAL();
			if(ret){
				record_no=RecordN+1;
			}else{
				record_no++;
			}
		}else{
			record_no=RecordN+1;
		}
	}
}


/*
recode_no			数据区记录状态
	0~RecordN		数据区内记录的数量
	>RecordN		不确定数据区的内容
	
before:写入数据放在record_buf
after:更新recode_no

*/
void set_record(void)
{
	u8 ret=0;
	if((record_no>=RecordN)||(!record_no)){
		ENTER_CRITIAL();
		APROM_Unlock();
		ret=IAP_ErasePage(KeyAddr);
		APROM_Lock();
		EXIT_CRITIAL();
		if(!ret){
			record_no=0;
		}else{
			record_no=RecordN+1;
			return;
		}
	}
	ENTER_CRITIAL();
	APROM_Unlock();
	ret=write_record_buf();
	APROM_Lock();
	EXIT_CRITIAL();
	if(!ret){
		record_no++;
	}else{
		record_no=RecordN+1;
	}
}
