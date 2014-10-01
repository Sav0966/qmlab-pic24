#ifndef _BUFFER_INCL_
#define _BUFFER_INCL_

#define _QUEBUF(id, size)\
 volatile que_##id##_buf[size] __attribute__((noload));\
 static struct {\
	volatile typeof(que_##id##_buf[0])* back; /* pointer to tail */\
	volatile typeof(que_##id##_buf[0])* front; /* pointer to head */\
	volatile int len; } que_##id __attribute__((near)) =\
	{que_##id##_buf, que_##id##_buf, 0}

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

typedef struct _LIST_NODE {
	struct _LIST_NODE *prev;
	struct _LIST_NODE *next;
} LIST_NODE; // List Item

#define _LIST(id)	LIST_NODE id##_list
#define LIST(id)	_LIST(id) // List descriptor

#define _LIST_FIRST(id)	id##_list.next
#define _LIST_LAST(id)	id##_list.prev
#define LIST_FIRST(id)	_LIST_FIRST(id)
#define LIST_LAST(id)	_LIST_LAST(id)

#define _LIST_GET(pos)	((LIST_NODE*)(pos)) // May be NULL !!!
#define LIST_GET(pos)	_LIST_GET(pos)

#define _LIST_NEXT(id, pos) ((pos)? (pos)->next: LIST_FIRST(id))
#define _LIST_PREV(id, pos) ((pos)? (pos)->prev: LIST_LAST(id))
#define LIST_NEXT(id, pos)	_LIST_NEXT(id, LIST_GET(pos))
#define LIST_PREV(id, pos)	_LIST_PREV(id, LIST_GET(pos))

#define _LIST_INIT(id)\
{ LIST_FIRST(id) = LIST_LAST(id) = 0; } ((void)0)
#define LIST_INIT(id)	_LIST_INIT(id)

#define _LIST_EMPTY(id)	(LIST_FIRST(id) == 0)
#define LIST_EMPTY(id)	_LIST_EMPTY(id)

#define _LIST_INSR(id, pos, node) { /* Insert after pos */	\
	register struct _LIST_NODE *_pos = LIST_GET(pos);		\
	if (!((node)->prev = _pos)) _pos = &id##_list; /* 1st */\
	if (!((node)->next = _pos->next)) LIST_LAST(id) = node;	\
	else (node)->next->prev = node;  /* Link to next item */\
	_pos->next = node; /* And LIST_FIRST() if (pos == 0) */	\
} ((void)0)
#define LIST_INSR(id, pos, node) _LIST_INSR(id, pos, node)

#define _LIST_INSL(id, pos, node) { /* Insert before pos */	\
	register struct _LIST_NODE *_pos = LIST_GET(pos);		\
	if (!((node)->next = _pos)) _pos = &id##_list;/* Last */\
	if (!((node)->prev = _pos->prev)) LIST_FIRST(id) = node;\
	else (node)->prev->next = node;  /* Link to next item */\
	_pos->prev = node; /* And LIST_LAST() if (pos == 0) */	\
} ((void)0)
#define LIST_INSL(id, pos, node) _LIST_INSL(id, pos, node)

#define _LIST_DEL(id, node) { /* Delete item from list-id */\
	if ((node)->prev) (node)->prev->next = (node)->next;	\
	else { LIST_FIRST(id) = (node)->next; } /* New first */	\
	if ((node)->next) (node)->next->prev = (node)->prev;	\
	else { LIST_LAST(id) = (node)->prev; } /* New last */	\
} ((void)0)
#define LIST_DEL(id, node) _LIST_DEL(id, node)

#define _LIST_ITEM(type) struct { type data; LIST_NODE node; }
#define _LIST_DATA(type, node)	((type*)((void*)(node) -	\
		 (sizeof(LIST_ITEM(type)) - sizeof(LIST_NODE))))
#define _LIST_NODE(type, item) ((LIST_NODE*)((void*)(item) +	\
		(sizeof(LIST_ITEM(type)) - sizeof(LIST_NODE))))
#define LIST_DATA(type, node)	_LIST_DATA(type, node)
#define LIST_NODE(type, item)	_LIST_NODE(type, item)
#define LIST_ITEM(type)			_LIST_ITEM(type)

#endif /*_BUFFER_INCL_*/
