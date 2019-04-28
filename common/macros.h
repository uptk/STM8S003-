#ifndef	__macros_h__
#define __macros_h__

#define	BIT0			(1<<0)
#define	BIT1			(1<<1)
#define	BIT2			(1<<2)
#define	BIT3			(1<<3)
#define	BIT4			(1<<4)
#define	BIT5			(1<<5)
#define	BIT6			(1<<6)
#define	BIT7			(1<<7)

#define	BIT8			((u16)1<<8)
#define	BIT9			((u16)1<<9)

#define	BITX(n)		(1<<(n))

#define	arraySizeOf(a)		(sizeof(a)/(sizeof(a[0])))

#ifndef	NULL
#define	NULL			((void *)0)
#endif
#endif
