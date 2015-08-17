/*
 * LibSunshine
 *
 * Function & type definitions for linked list implementation.
 */

#ifndef List_h
#define List_h

#ifdef __cplusplus
extern "C" {
#endif

#include <threads.h>

typedef struct List_s_
{
    void * data;
    struct List_s_ * Link;
} List_t_;

typedef struct List_s
{
    List_t_ * List;
    mtx_t Lock;
} List_t;

List_t * List_new ();

void List_add (List_t * n, void * data);
void List_del (List_t * n, void * data);
void List_print (List_t * n);

void * List_retrieve_and_remove_first (List_t * n);

#define LIST_ITERATE_OPEN(list)                                                \
    List_t_ * tmp;                                                             \
    void * e_data;                                                             \
    mtx_lock (&(list)->Lock);                                                  \
    for (List_t_ * e = (list)->List;                                           \
         (e) && (tmp = (e)->Link, 1) && (e_data = e->data); (e) = tmp)         \
    {

#define LIST_ITERATE_CLOSE(list)                                               \
    }                                                                          \
    mtx_unlock (&(list)->Lock);

#ifdef __cplusplus
}
#endif

#endif
