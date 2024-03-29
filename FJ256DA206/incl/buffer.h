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

#define _QUEBUF(id, size)\
 que_##id##_buf[size] __attribute__((noload));\
 static struct {\
	volatile typeof(que_##id##_buf[0])* back; /* pointer to tail */\
	volatile typeof(que_##id##_buf[0])* front; /* pointer to head */\
	volatile int len; } que_##id = {que_##id##_buf, que_##id##_buf, 0}

#define QUEBUF(id, size)	_QUEBUF(id, size)

// Assistant macroses

#define _QUEBUF_SIZE(id)\
 (sizeof(que_##id##_buf)/sizeof(que_##id##_buf[0]))
#define _QUEBUF_END(id) &que_##id##_buf[_QUEBUF_SIZE(id)]
#define _QUEBUF_TYPE(id) typeof(que_##id##_buf[0])

#define _QUEBUF_LEN(id)			((int)que_##id.len)
#define _QUEBUF_FRONTREF(id)	(*que_##id.front)

#define _QUEBUF_MOVEFD(id) /* Not locked, only one thread can dequeue */\
 if (++que_##id.front == _QUEBUF_END(id)) que_##id.front = que_##id##_buf

#define _QUEBUF_MOVEBK(id) /* Not locked, only one thread can enqueue */\
 if (++que_##id.back == _QUEBUF_END(id)) que_##id.back = que_##id##_buf

// Locked, any thread can change length of the buffer
#define _QUEBUF_DEC_LEN(id) INTERLOCKED(--que_##id.len)
#define _QUEBUF_INC_LEN(id) INTERLOCKED(++que_##id.len)

// First element at the front of the queue
#define QUEBUF_FRONTREF(id)	_QUEBUF_FRONTREF(id)
#define QUEBUF_FRONT(id)    ((_QUEBUF_TYPE(id))QUEBUF_FRONTREF(id))

#define _QUEBUF_BACK(id)\
 /* Last and most recently added element at the back of the queue */\
 (que_##id.back != que_##id##_buf)? (*(que_##id.back-1)): (*(_QUEBUF_END(id)-1))
#define QUEBUF_BACK(id)	((_QUEBUF_TYPE(id)_QUEBUF_BACK(id))

// Interface

#define QUEBUF_LEN(id)		_QUEBUF_LEN(id)
#define QUEBUF_SIZE(id)		_QUEBUF_SIZE(id)
#define QUEBUF_EMPTY(id)	(QUEBUF_LEN(id) == 0)
#define QUEBUF_FULL(id)		(QUEBUF_LEN(id) == QUEBUF_SIZE(id))

#define QUEBUF_POP(id)\
	QUEBUF_FRONT(id); /* Removes an element from the front of the queue */\
	if (!QUEBUF_EMPTY(id)) {_QUEBUF_DEC_LEN(id); _QUEBUF_MOVEFD(id);}

#define _QUEBUF_IPOP(id)\
	QUEBUF_FRONT(id); /* Not blocked - can be called in ISR only */\
	if (!QUEBUF_EMPTY(id)) {--que_##id.len; _QUEBUF_MOVEFD(id);}
#define QUEBUF_IPOP(id)	_QUEBUF_IPOP(id)

#define _QUEBUF_PUSH(id, val) if (!QUEBUF_FULL(id))\
	{ /* Adds an element to the back of the queue */\
		*que_##id.back = (_QUEBUF_TYPE(id))(val); \
		_QUEBUF_INC_LEN(id); _QUEBUF_MOVEBK(id); }
#define QUEBUF_PUSH(id, val)	_QUEBUF_PUSH(id, val)

#define _QUEBUF_IPUSH(id, val)	if (!QUEBUF_FULL(id))\
	{ /* Not blocked - can be called in ISR only */\
		*que_##id.back = (_QUEBUF_TYPE(id))(val);\
		++que_##id.len; _QUEBUF_MOVEBK(id); }
#define QUEBUF_IPUSH(id, val)	_QUEBUF_IPUSH(id, val)

// Reset queue to Empty state (must be called from dequeue thread)
#define QUEBUF_RESET(id) while (!QUEBUF_EMPTY(id)) QUEBUF_POP(id)

// Don't reset 'len' & 'back' directly if you don't sure with your right
#define __QUEBUF_INIT(id) que_##id.back = que_##id.front; que_##id.len = 0
#define _QUEBUF_INIT(id) __QUEBUF_INIT(id) /* Not locked function */
#define QUEBUF_INIT(id)  INTERLOCKED(_QUEBUF_INIT(id)) /* Locked */

#endif /*_BUFFER_INCL_*/
