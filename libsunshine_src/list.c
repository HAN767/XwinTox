/* 
 * LibSunshine
 * 
 * Implementation of linked lists.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "list.h"

void 
List_add(List_t **n, void* data)
{
	List_t *temp,*t;
	if(*n==NULL)
	{
		/* create new list */
		t=malloc(sizeof(List_t));
		t->data=data;
		t->Link=NULL;
		_Lock_Init(t->_Lock);
		*n=t;
	}
	else
	{
		t=*n;
		temp=malloc(sizeof(List_t));
		_Locked_Start(t->_Lock)
		_Locktype_ lock; //=calloc(1, sizeof(_Locktype_));
		lock =t->_Lock;
		while(t->Link!=NULL)
			t=t->Link;
		temp->data=data;
		temp->Link=NULL;
		t->Link=temp;
		t->_Lock =lock;
		_Locked_End(t->_Lock)
	}
}

void
List_del(List_t **list, void* data)
{
	List_t *current, *previous;

	previous = NULL;

	_Locked_Start((*list)->_Lock)
	for (current = *list;
	        current != NULL;
	        previous = current, current = current->Link)
	{
		if (current->data == data)
		{
			if (previous == NULL)
			{
				// correct the first
				*list = current->Link;
			}
			else
			{
				// skip 
				previous->Link = current->Link;
			}
			
			_Locked_End((*list)->_Lock)
			free(current);
			return;
		}
	}
}

void 
List_print(List_t **n)
{
	List_t *t;
	t=*n;
	if(t==NULL) printf("Empty list\n");
	else
	{
		printf("Begin list.\n");
		_Locked_Start(t->_Lock)
		while(t!=NULL)
		{
			printf("%d\n",(int)t->data);
			t=t->Link;
		}
		_Locked_End(t->_Lock)
		printf("End list.\n");
	}
	return;
}

void*
List_retrieve_and_remove_first(List_t **n)
{
	void *ret;
	List_t *tmp;
	_Locktype_ tmplock;

	if(*n==NULL)
	{
		printf("No list exists\n"); return 0;
	}
	else
	{
		_Locked_Start((*n)->_Lock)
		ret =(*n)->data;
		if ((*n)->Link)
		{
			tmp =(*n)->Link;
			tmplock =(*n)->_Lock;
			tmp->_Lock =tmplock;
			free(*n);
			*n =tmp;
			_Locked_End((*n)->_Lock)
		}
		else
		{
			_Locked_End((*n)->_Lock)
			free (*n);
			*n =NULL;
		}

		return ret;
	}
}
