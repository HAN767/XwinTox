#ifndef __shared_ptr_H__
#define __shared_ptr_H__

#include <stdlib.h>
#include <threads.h>

typedef struct Shared_Ptr_s
{
	void *pvData;

	unsigned int wCnt;
	mtx_t mtxCnt;
} Shared_Ptr_t;

static inline Shared_Ptr_t* Shared_Ptr_new(void *pvData)
{
	Shared_Ptr_t *psprNew =(Shared_Ptr_t *)malloc(sizeof(Shared_Ptr_t));
	
	psprNew->pvData =pvData;
	psprNew->wCnt =1;
	mtx_init(&psprNew->mtxCnt, mtx_plain);

	return psprNew;
}

static inline void Shared_Ptr_inc(Shared_Ptr_t *psprSptr)
{
	mtx_lock(&psprSptr->mtxCnt);
	psprSptr->wCnt +=1;
	mtx_unlock(&psprSptr->mtxCnt);
}

static inline void Shared_Ptr_dec(Shared_Ptr_t *psprSptr)
{
	mtx_lock(&psprSptr->mtxCnt);
	psprSptr->wCnt -=1;
	if(psprSptr->wCnt == 0)
	{
		free(psprSptr->pvData);
		free(psprSptr);
	}
	else
	{
		mtx_unlock(&psprSptr->mtxCnt);
	}
	return;
}

#endif

