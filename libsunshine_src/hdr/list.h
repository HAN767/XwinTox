/* 
 * LibSunshine
 * 
 * Function & type definitions for linked list implementation.
 */

#ifndef List_h
#define List_h

#include "misc.h" /* threading support */

typedef struct List
{
    void* data; // change
    struct List *Link;
	_Lock_
} List_t;

void List_add(List_t **n, void *data);
void List_del(List_t **n, void *data);
void List_print(List_t **n);

/* list_iterate is not thread safe */
#define LIST_ITERATE(list) \
	List_t *tmp; \
	void *e_data; \
	for(List_t *e=(list);(e) && (tmp = (e)->Link, 1) && (e_data =e->data); (e) = tmp)

#define LIST_ITERATE_OPEN(list) \
	List_t *tmp; \
	void *e_data; \
	_Locked_Start(( list )->_Lock) \
	for(List_t *e=(list);(e) && (tmp = (e)->Link, 1) && (e_data =e->data); (e) = tmp)

#define LIST_ITERATE_CLOSE(list) _Locked_End(( list )->_Lock); }

#endif
