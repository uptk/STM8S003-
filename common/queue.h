#ifndef	__queue_h__
#define	__queue_h__

/*
	此文件包含在 common.h中，故这里不include <common.h>
*/

#define	QueueBufSize		65
typedef struct{
	u8 buf[QueueBufSize];
	u8 front;
	u8 rear;
}queue_t;

extern void inQ(queue_t *q,u8 d);
extern u8 outQ(queue_t *q);
extern void initQ(queue_t *q);
extern u8 is_emptyQ(queue_t *q);
extern u8 is_fullQ(queue_t *q);
extern u8 usedQ(queue_t *q);
extern u8 leftQ(queue_t *q);
#endif
