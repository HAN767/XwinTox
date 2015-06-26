#include <threads.h>
#include <tox/toxdns.h>

#include "misc.h"
#include "list.h"

#include "xwintox.h"

void Resolv_main()
{
	/* now wait until we are needed */
	while (mtx_lock(&APP->Resolv->Run) == thrd_success)
	{
		Call_t *call;
		dbg("Work cycle\n")
		if(APP->Resolv->Calls->List) while((call =List_retrieve_and_remove_first(
												  APP->Resolv->Calls)) != 0)
		{
			dbg("Call: %d\n", call->Func);
		} 
	}
}
