#ifndef	__INIT_H__
#define	__INIT_H__

#include "common.h"


extern void CLK_8M_init(void);
extern void TIM4_1MS_init(void);
extern void IWDG_500ms(void) ;


extern void Uart2_Init(void);
extern void Uart2_SendByte(u8 data);
extern void Uart2_SendStr(u8 *buf,u8 len);


extern volatile bool irq_enable;
extern void ENTER_CRITIAL(void);
extern void EXIT_CRITIAL(void);
extern void enable_irq(void);
extern void disable_irq(void);



/*	¼ÇÂ¼	

´æ´¢								0					1				3				4			...				RecordLen-1
KeyAddr+0							RecordKey+0		~(RecordKey+0)		D00				D01			...				D0(RecordLen-3)
KeyAddr+RecordLenB					RecordKey+1		~(RecordKey+1)		D10				D11			...				D1(RecordLen-3)
KeyAddr+2*RecordLenB				RecordKey+2		~(RecordKey+2)		D20				D21			...				D2(RecordLen-3)
...									...
...									...
KeyAddr+PageSize-RecordLenB			...
*/
	
#define	APROM_Unlock()	FLASH_Unlock(FLASH_MEMTYPE_DATA)
#define	APROM_Lock()	FLASH_Lock(FLASH_MEMTYPE_DATA)
	
#define	PageSize	64
#define	PROG_t		u8
#define	EraseFill	((PROG_t)0xff)
#define	RecordKey	((PROG_t)0x56)
#define	KeyCS		((PROG_t)0)

#define	KeyAddr			((u16)0x4000)
#define	RecordLen		(RecordLenB/sizeof(PROG_t))
#define	RecordLenB		16
#define	RecordN			(PageSize/RecordLenB)//RecordLen*sizeof(PROG_t)*RecordN = PageSize
#define	RecordBufSize	arraySizeOf(record_buf)

extern u8 record_no;
extern NEAR PROG_t record_buf[RecordLen-2];
extern void get_record(void);
extern void reset_record(void);
extern void set_record(void);
#endif
