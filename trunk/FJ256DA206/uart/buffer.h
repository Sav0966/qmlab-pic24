#ifndef _BUFFER_INCL_
#define _BUFFER_INCL_

// Thread synchronization

#ifndef INTERLOCKED_INCREMENT // Blocked operation with integer
#define INTERLOCKED_INCREMENT(i) __asm__ volatile ("disi #3"); ++i
#define INTERLOCKED_DECREMENT(i) __asm__ volatile ("disi #3"); --i
#endif

// Queue template (one thread enqueues, another - dequeues)

#define _QUEUE(id, size)\
 que_##id##_buf[size] __attribute__((noload));\
 static struct {\
	typeof(que_##id##_buf[0])* volatile back; /* pointer to tail */\
	typeof(que_##id##_buf[0])* volatile front; /* pointer to head */\
	volatile int len; } que_##id = {que_##id##_buf, que_##id##_buf, 0}

#define QUEUE(id, size)	_QUEUE(id, size)

// Assistant macroses

#define _QUE_BUF_SIZE(id)\
 (sizeof(que_##id##_buf)/sizeof(que_##id##_buf[0]))
#define _QUE_BUF_END(id) &que_##id##_buf[_QUE_BUF_SIZE(id)]
#define _QUE_TYPE(id) typeof(que_##id##_buf[0])

#define _QUE_SIZE(id)	((int)que_##id.len)
#define _QUE_FRONTREF(id) (*que_##id.front)

#define _QUE_MOVE_FRONT(id) /* Not locked, only one thread can dequeue */\
 if (++que_##id.front == _QUE_BUF_END(id)) que_##id.front = que_##id##_buf

#define _QUE_MOVE_BACK(id) /* Not locked, only one thread can enqueue */\
 if (++que_##id.back == _QUE_BUF_END(id)) que_##id.back = que_##id##_buf

// Locked, one thread can enqueue, another - dequeue
#define _QUE_DEC_SIZE(id)	INTERLOCKED_DECREMENT(que_##id.len)
#define _QUE_INC_SIZE(id)	INTERLOCKED_INCREMENT(que_##id.len)

// Interface

#define QUE_SIZE(id)	_QUE_SIZE(id)
#define QUE_EMPTY(id)	(QUE_SIZE(id) == 0)
#define QUE_FULL(id)	(QUE_SIZE(id) == _QUE_BUF_SIZE(id))

// First element at the front of the queue
#define QUE_FRONTREF(id)	_QUE_FRONTREF(id)
#define QUE_FRONT(id)    ((_QUE_TYPE(id))QUE_FRONTREF(id))

#define QUE_POP(id) /* Removes an element from the front of the queue */\
 QUE_FRONT(id); if (!QUE_EMPTY(id)) { _QUE_DEC_SIZE(id); _QUE_MOVE_FRONT(id); }

#define _QUE_PUSH(id, val) /* Adds an element to the back of the queue */\
 if (!QUE_FULL(id)) { *que_##id.back = val; _QUE_INC_SIZE(id); _QUE_MOVE_BACK(id); }
#define QUE_PUSH(id, val)	_QUE_PUSH(id, val)

#define _QUE_BACK(id)\
 /* Last and most recently added element at the back of the queue */\
 (que_##id.back != que_##id##_buf)? (*(que_##id.back-1)): (*(_QUE_BUF_END(id)-1))
#define QUE_BACK(id)	_QUE_BACK(id)

// Reset queue to Empty state (called from dequeue thread)
#define QUE_RESET(id)	while (!QUE_EMPTY(id)) QUE_POP(id)

// Don't reset 'len' & 'back' directly if you don't sure with your right
#define __QUE_INIT(id) que_##id.back = que_##id.front; que_##id.len = 0
#define _QUE_INIT(id)	__QUE_INIT(id)

#endif /*BUFFER_INCL_*/
