#ifndef _BUFFER_INCL_
#define _BUFFER_INCL_

// Thread synchronization
#ifndef DISABLE_INTERRUPT
// 0-6 int level disabling
#define DISABLE_INTERRUPT()\
	{	register int _disicnt_to_save = DISICNT;\
		__asm__ volatile ("disi #0x3FFF")

#define ENABLE_INTERRUPT() DISICNT = _disicnt_to_save; } ((void)0)

#define INTERLOCKED(f) DISABLE_INTERRUPT(); f; ENABLE_INTERRUPT()
#endif // DISABLE_INTERRUPT

// Queue template (one thread can enqueues, another - dequeues)
// The queue is implemented as an array of needed type of elements

#define _QUE_BUF(id, size)\
 que_##id##_buf[size] __attribute__((noload));\
 static struct {\
	typeof(que_##id##_buf[0])* volatile back; /* pointer to tail */\
	typeof(que_##id##_buf[0])* volatile front; /* pointer to head */\
	volatile int len; } que_##id = {que_##id##_buf, que_##id##_buf, 0}

#define QUEBUF(id, size)	_QUE_BUF(id, size)

// Assistant macroses

#define _QUE_BUF_SIZE(id)\
 (sizeof(que_##id##_buf)/sizeof(que_##id##_buf[0]))
#define _QUE_BUF_END(id) &que_##id##_buf[_QUE_BUF_SIZE(id)]
#define _QUE_BUF_TYPE(id) typeof(que_##id##_buf[0])

#define _QUE_BUF_LEN(id)		((int)que_##id.len)
#define _QUE_BUF_FRONTREF(id)	(*que_##id.front)

#define _QUE_BUF_MOVEFD(id) /* Not locked, only one thread can dequeue */\
 if (++que_##id.front == _QUE_BUF_END(id)) que_##id.front = que_##id##_buf

#define _QUE_BUF_MOVEBK(id) /* Not locked, only one thread can enqueue */\
 if (++que_##id.back == _QUE_BUF_END(id)) que_##id.back = que_##id##_buf

// Locked, any thread can change length of the buffer
#define _QUE_BUF_DEC_LEN(id) INTERLOCKED(--que_##id.len)
#define _QUE_BUF_INC_LEN(id) INTERLOCKED(++que_##id.len)

// First element at the front of the queue
#define QUE_BUF_FRONTREF(id)	_QUE_BUF_FRONTREF(id)
#define QUE_BUF_FRONT(id)    ((_QUE_BUF_TYPE(id))QUE_BUF_FRONTREF(id))

#define _QUE_BUF_BACK(id)\
 /* Last and most recently added element at the back of the queue */\
 (que_##id.back != que_##id##_buf)? (*(que_##id.back-1)): (*(_QUE_BUF_END(id)-1))
#define QUE_BUF_BACK(id)	((_QUE_BUF_TYPE(id)_QUE_BUF_BACK(id))

// Interface

#define QUE_BUF_LEN(id)		_QUE_BUF_LEN(id)
#define QUE_BUF_SIZE(id)	_QUE_BUF_SIZE(id)
#define QUE_BUF_EMPTY(id)	(QUE_BUF_LEN(id) == 0)
#define QUE_BUF_FULL(id)	(QUE_BUF_LEN(id) == QUE_BUF_SIZE(id))

#define QUE_BUF_POP(id)\
	QUE_BUF_FRONT(id); /* Removes an element from the front of the queue */\
	if (!QUE_BUF_EMPTY(id)) {_QUE_BUF_DEC_LEN(id); _QUE_BUF_MOVEFD(id);}

#define _QUE_BUF_IPOP(id)\
	QUE_BUF_FRONT(id); /* Not blocked - can be called in ISR only */\
	if (!QUE_BUF_EMPTY(id)) {--que_##id.len; _QUE_BUF_MOVEFD(id);}
#define QUE_BUF_IPOP(id)	_QUE_BUF_IPOP(id)

#define _QUE_BUF_PUSH(id, val) if (!QUE_BUF_FULL(id))\
	{ /* Adds an element to the back of the queue */\
		*que_##id.back = (_QUE_BUF_TYPE(id))(val); \
		_QUE_BUF_INC_LEN(id); _QUE_BUF_MOVEBK(id); }
#define QUE_BUF_PUSH(id, val)	_QUE_BUF_PUSH(id, val)

#define _QUE_BUF_IPUSH(id, val)	if (!QUE_BUF_FULL(id))\
	{ /* Not blocked - can be called in ISR only */\
		*que_##id.back = (_QUE_BUF_TYPE(id))(val);\
		++que_##id.len; _QUE_BUF_MOVEBK(id); }
#define QUE_BUF_IPUSH(id, val)	_QUE_BUF_IPUSH(id, val)

// Reset queue to Empty state (must be called from dequeue thread)
#define QUE_BUF_RESET(id) while (!QUE_BUF_EMPTY(id)) QUE_BUF_POP(id)

// Don't reset 'len' & 'back' directly if you don't sure with your right
#define __QUE_BUF_INIT(id) que_##id.back = que_##id.front; que_##id.len = 0
#define _QUE_BUF_INIT(id) __QUE_BUF_INIT(id) /* Not locked function */
#define QUE_BUF_INIT(id)  INTERLOCKED(_QUE_BUF_INIT(id)); /* Locked */

#endif /*BUFFER_INCL_*/
