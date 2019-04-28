#ifndef	__common_h__
#define	__common_h__


#include "stm8s.h"
#include "STM8S005K6.h"
#include "macros.h"
#include "queue.h"
#include "safeSetVal.h"

extern void Mymemcpy(u8 *dst,u8 *src,u8 n);
extern u8 Mymemcmp(u8 *dst,u8 *src,u8 n);
extern u8 checkSum(u8 *s,u8 n);
extern u8 cal_1(u8 val);
extern void nopDelay(u8 val);
extern void  delay_us(u8 us);
extern void delay_ms(u8 ms);
extern s8 rawBinSearch16(u16 const *a,u8 len,u16 ele);
extern s8 rawBinSearch(u8 const *a,u8 len,u8 ele);
extern u8 binSearch16(u16 const *a,u8 len,u16 ele);
extern u8 binSearch(u8 const *a,u8 len,u8 ele);
extern u16 getMid16(u16 *buf);
extern u8 tryGetInc(u8 rold,u8 rnew);
extern unsigned char ACRC[2];
extern void Acrc16(unsigned char *ptr,unsigned int len);


#endif
