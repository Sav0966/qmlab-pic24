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

#ifndef INTERLOCKED_INCREMENT
#define INTERLOCKED_INCREMENT(i) __asm__ volatile ("disi #3"); ++i
#define INTERLOCKED_DECREMENT(i) __asm__ volatile ("disi #3"); --i
#endif

#define _QUE_MOVE_FRONT(id) /* Not lock, only one thread can dequeue */\
 if (++que_##id.front == _QUE_BUF_END(id)) que_##id.front = que_##id##_buf

#define _QUE_MOVE_BACK(id) /* Not lock, only one thread can enqueue */\
 if (++que_##id.back == _QUE_BUF_END(id)) que_##id.back = que_##id##_buf

// One thread can enqueue, another - dequeue
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

#define QUE_PUSH(id, val) /* Adds an element to the back of the queue */\
 if (!QUE_FULL(id)) { *que_##id.back = val; _QUE_INC_SIZE(id); _QUE_MOVE_BACK(id); }

// Last and most recently added element at the back of the queue
//#define QUE_BACKREF(id)  (*que_##id.back)
//#define QUE_BACK(id)  ((_QUE_TYPE(id))QUE_BACKREF(id))

#endif /*BUFFER_INCL_*/
