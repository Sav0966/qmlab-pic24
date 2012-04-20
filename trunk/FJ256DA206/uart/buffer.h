#ifndef _BUFFER_INCL_
#define _BUFFER_INCL_

// Queue template (one thread enqueues, another - dequeues)

#define QUEUE(id, size)\
 que_##id##_buf[size] __attribute__((noload));\
 static struct {\
	typeof(que_##id##_buf[0])* volatile back; /* pointer to tail */\
	typeof(que_##id##_buf[0])* volatile front; /* pointer to head */\
	volatile int len; } que_##id = {que_##id##_buf, que_##id##_buf, 0}

// Assistant macroses

#define _QUE_BUF_SIZE(id)\
 (sizeof(que_##id##_buf)/sizeof(que_##id##_buf[0]))
#define _QUE_BUF_END(id) &que_##id##_buf[_QUE_BUF_SIZE(id)]
#define _QUE_TYPE(id) typeof(que_##id##_buf[0])

// Thread synchronization

#ifndef INTERLOCKED_INCREMENT // Blocked operation with integer
#define INTERLOCKED_INCREMENT(i) __asm__ volatile ("disi #3"); ++i
#define INTERLOCKED_DECREMENT(i) __asm__ volatile ("disi #3"); --i
#endif

#define _QUE_MOVE_FRONT(id) /* Not locked, only one thread can dequeue */\
 if (++que_##id.front == _QUE_BUF_END(id)) que_##id.front = que_##id##_buf

#define _QUE_MOVE_BACK(id) /* Not locked, only one thread can enqueue */\
 if (++que_##id.back == _QUE_BUF_END(id)) que_##id.back = que_##id##_buf

// Locked, one thread can enqueue, another - dequeue
#define _QUE_DEC_SIZE(id)	INTERLOCKED_DECREMENT(que_##id.len)
#define _QUE_INC_SIZE(id)	INTERLOCKED_INCREMENT(que_##id.len)

// Interface

#define QUE_SIZE(id)	((int)que_##id.len)
#define QUE_EMPTY(id)	(que_##id.len == 0)
#define QUE_FULL(id)	(que_##id.len == _QUE_BUF_SIZE(id))

// First element at the front of the queue
#define QUE_FRONTREF(id) (*que_##id.front)
#define QUE_FRONT(id)    ((_QUE_TYPE(id))QUE_FRONTREF(id))

#define QUE_POP(id) /* Removes an element from the front of the queue */\
 QUE_FRONT(id); if (!QUE_EMPTY(id)) { _QUE_DEC_SIZE(id); _QUE_MOVE_FRONT(id); }

// Reset queue to Empty state (called from dequeue thread)
#define QUE_RESET(id)	while (!QUE_EMPTY(id)) QUE_POP(id)
// Don't reset 'len' directly if you don't sure with your right

#define QUE_PUSH(id, val) /* Adds an element to the back of the queue */\
 if (!QUE_FULL(id)) { *que_##id.back = val; _QUE_INC_SIZE(id); _QUE_MOVE_BACK(id); }

#define QUE_BACK(id)\
 /* Last and most recently added element at the back of the queue */\
 (que_##id.back != que_##id##_buf)? (*(que_##id.back-1)): (*(_QUE_BUF_END(id)-1))

#endif /*BUFFER_INCL_*/
