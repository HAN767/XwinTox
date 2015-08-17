#include <stdlib.h>
#include <threads.h>

#include "misc.h"
#include "list.h"
#include "postbox.h"

typedef struct PB_Thread_Msg_s
{
    int mtype;
    Shared_Ptr_t * msg;
    PB_Callback_f fnCB;
    void * pvCustom;
} PB_Thread_Msg_t;

static void PBFilter (Postbox_t * pb, unsigned int mtype, PBMessage_t * msg)
{
    if (msg->filtered == 0)
    {
        LIST_ITERATE_OPEN (pb->filters)
        if (((PBRegistryEntry_t *)e_data)->mtype == mtype)
        {
            dbg ("MTYPE: %d\n Msg: %p\n", mtype, msg);
            ((PBRegistryEntry_t *)e_data)
                ->callback (mtype, msg, ((PBRegistryEntry_t *)e_data)->custom);
        }
        LIST_ITERATE_CLOSE (pb->filters)

        msg->filtered = 1;
    }
}

int PBThread_Main (void * custom)
{
    PBThread_t * self = custom;
    PB_Thread_Msg_t * msg = 0;

    while (1)
    {
        mtx_lock (&self->msgMtx);

        while (!self->msgCnt)
        {
            cnd_wait (&self->msgCnd, &self->msgMtx);
        }

        mtx_unlock (&self->msgMtx);

        while ((msg = List_retrieve_and_remove_first (self->msgQueue)) != 0)
        {
            PBFilter (self->postbox, msg->mtype, msg->msg->pvData);
            msg->fnCB (msg->mtype, msg->msg->pvData, msg->pvCustom);

            mtx_lock (&self->msgMtx);
            self->msgCnt -= 1;
            mtx_unlock (&self->msgMtx);

            PBM_DEC (msg->msg);
            free (msg);
        }
    }
}

Postbox_t * PB_New ()
{
    Postbox_t * newpb = calloc (1, sizeof (Postbox_t));
    newpb->clients = List_new ();
    newpb->deferred = List_new ();
    newpb->filters = List_new ();
    mtx_init (&newpb->Lock, mtx_plain);

    for (int i = 0; i < 3; i++)
    {
        mtx_init (&newpb->threads[i].msgMtx, mtx_plain);
        cnd_init (&newpb->threads[i].msgCnd);
        newpb->threads[i].msgCnt = 0;
        newpb->threads[i].msgQueue = List_new ();
        newpb->threads[i].postbox = newpb;
        thrd_create (&newpb->threads[i].thrd, PBThread_Main,
                     &newpb->threads[i]);
    }

    return newpb;
}

void PB_Signal (Postbox_t * pb, int mtype, PBMessage_t * msg)
{
    LIST_ITERATE_OPEN (pb->clients)

    if (((PBRegistryEntry_t *)e_data)->mtype == mtype)
    {
        ((PBRegistryEntry_t *)e_data)
            ->callback (mtype, msg, ((PBRegistryEntry_t *)e_data)->custom);
    }

    LIST_ITERATE_CLOSE (pb->clients)

    PB_Free_Message (msg);
}

void PB_Signal_Multithreaded (Postbox_t * pb, int mtype, PBMessage_t * msg)
{
    Shared_Ptr_t * sprMsg = Shared_Ptr_new (msg);

    LIST_ITERATE_OPEN (pb->clients)
    if (((PBRegistryEntry_t *)e_data)->mtype == mtype)
    {
        PBM_INC (sprMsg);
        PB_Thread_Msg_t * pbtMsg = malloc (sizeof (PB_Thread_Msg_t));
        unsigned int dwThread;

        if (msg->P == PB_Slow)
        {
            dbg ("slow\n");
            dwThread = 0;
        }
        else if (msg->P == PB_Fast)
            dwThread = 1;
        else
            dwThread = 2;

        pbtMsg->mtype = mtype;
        pbtMsg->msg = sprMsg;
        pbtMsg->fnCB = ((PBRegistryEntry_t *)e_data)->callback;
        pbtMsg->pvCustom = ((PBRegistryEntry_t *)e_data)->custom;

        mtx_lock (&pb->threads[dwThread].msgMtx);
        List_add (pb->threads[dwThread].msgQueue, pbtMsg);
        pb->threads[dwThread].msgCnt += 1;
        cnd_broadcast (&pb->threads[dwThread].msgCnd);
        mtx_unlock (&pb->threads[dwThread].msgMtx);
    }
    LIST_ITERATE_CLOSE (pb->clients)

    PBM_DEC (sprMsg);
}

void PB_Despatch_Deferred (Postbox_t * pb)
{
    PBDeferred_t * d = 0;

    while ((d = List_retrieve_and_remove_first (pb->deferred)) != 0)
    {
        LIST_ITERATE_OPEN (pb->clients)

        if (((PBRegistryEntry_t *)e_data)->mtype == d->mtype)
        {
            ((PBRegistryEntry_t *)e_data)
                ->callback (d->mtype, d->msg,
                            ((PBRegistryEntry_t *)e_data)->custom);
        }

        LIST_ITERATE_CLOSE (pb->clients)

        PB_Free_Message (d->msg);
        free (d);
    }
}

void PB_Defer (Postbox_t * pb, int mtype, PBMessage_t * msg)
{
    PBDeferred_t * newdf = calloc (1, sizeof (PBDeferred_t));

    newdf->msg = msg;
    newdf->mtype = mtype;

    List_add (pb->deferred, newdf);
}

void PB_Register (Postbox_t * pb, int mtype, void * custom,
                  void (*callback)(int, PBMessage_t *, void *))
{
    PBRegistryEntry_t * e = calloc (1, sizeof (PBRegistryEntry_t));
    e->mtype = mtype;
    e->callback = callback;
    e->custom = custom;
    List_add (pb->clients, e);
}

void PB_Register_Filter (Postbox_t * pb, int mtype, void * custom,
                         void (*callback)(int, PBMessage_t *, void *))
{
    PBRegistryEntry_t * e = calloc (1, sizeof (PBRegistryEntry_t));
    e->mtype = mtype;
    e->callback = callback;
    e->custom = custom;
    List_add (pb->filters, e);
}

void PB_Deregister (Postbox_t * pb, int mtype, void * custom,
                    void (*callback)(int, PBMessage_t *, void *))
{
    PBRegistryEntry_t * en;
    LIST_ITERATE_OPEN (pb->clients)
    en = (PBRegistryEntry_t *)e_data;
    if (en->mtype == mtype && en->callback == callback && en->custom == custom)
        break;
    LIST_ITERATE_CLOSE (pb->clients)
    List_del (pb->clients, en);
}
