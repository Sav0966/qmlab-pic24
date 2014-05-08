#ifndef _BUFFER_INCL_
#define _BUFFER_INCL_

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

#define _QUEBUF_MOVEFD(id)\
 if (++que_##id.front == _QUEBUF_END(id)) que_##id.front = que_##id##_buf

#define _QUEBUF_MOVEBK(id)\
 if (++que_##id.back == _QUEBUF_END(id)) que_##id.back = que_##id##_buf

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

// Removes an element from the front of the queue
#define __QUEBUF_POP(id, ref) {\
	ref = QUEBUF_FRONT(id);\
	--que_##id.len; _QUEBUF_MOVEFD(id); } ((void)0)

#define _QUEBUF_POP(id, ref)	__QUEBUF_POP(id, ref)
#define QUEBUF_POP(id, ref)	if (!QUEBUF_EMPTY(id)) _QUEBUF_POP(id, ref)

// Adds an element to the back of the queue
#define __QUEBUF_PUSH(id, val) {\
		*que_##id.back = (_QUEBUF_TYPE(id))(val);\
		++que_##id.len; _QUEBUF_MOVEBK(id); } ((void)0)

#define _QUEBUF_PUSH(id, val)	__QUEBUF_PUSH(id, val)
#define QUEBUF_PUSH(id, val) if (!QUEBUF_FULL(id)) _QUEBUF_PUSH(id, val)

#define _QUEBUF_INIT(id) que_##id.back = que_##id.front; que_##id.len = 0
#define QUEBUF_INIT(id) _QUEBUF_INIT(id)

#endif /*_BUFFER_INCL_*/
