#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>

#include "misc.h"
#include "xdrcomm.h"
#include "xwintox.h"
#include "postbox.h"

#define RETRY \
	{ sleep (5); \
		goto reconnect; }

int readit(void* handle, void* buf, int len)
{
	int sockfd =*(int*)handle;
	int err =recv(sockfd, buf, len, 0);
	dbg("Err: %d\n", err);

	if(err == 0) err =-1;

	return err;
}

int writeit(void* handle, void* buf, int len)
{
	int sockfd =*(int*)handle;
	int err =send(sockfd, buf, len, 0);
	dbg("Err: %d\n", err);

	if(err == 0) err =-1;

	return err;
}

void Xdrcomm_despatchevent(Event_t *event)
{
	dbg("Tox event: T:%d, ID:%d, I1=%d\n", event->T, event->ID, event->I1,
	    event->S1);

	if(event->T == TREQUEST)
	{
		PBMessage_t *msg =calloc(1, sizeof(PBMessage_t));
		msg->I1 =event->ID;
		msg->I2 =event->I1;
		msg->I3 =event->I2;
		msg->S1 =strdup(event->S1);
		PB_Defer(APP->events, PB_TRequest, msg);
	}
	if(event->T == TCONTROL)
	{
		PBMessage_t *msg =calloc(1, sizeof(PBMessage_t));
		msg->I1 =event->ID;
		msg->I2 =event->I1;
		msg->I3 =event->I2;
		PB_Defer(APP->events, PB_TControl, msg);
	}

	Ev_free(event);
}

int Xdrcomm_main(void *data)
{
	Xdrcomm_t *Xdrcomm =data;
	int ready =0, err, zeroflag;
	struct addrinfo hints, *res, *resfree;
	char *remote ="localhost";
	fd_set recvfds;

reconnect:
	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_protocol=IPPROTO_TCP;

	if((err =getaddrinfo(remote, Xdrcomm->port, &hints, &res)) != 0)
	{
		dbg("getaddrinfo error for %s:%s; %s", remote, Xdrcomm->port, gai_strerror(err));
		RETRY
	}

	resfree=res;

	do
	{
		Xdrcomm->recv_fd =socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if(Xdrcomm->recv_fd < 0)
			continue;  /*ignore Ip addr*/

		if(connect(Xdrcomm->recv_fd, res->ai_addr, res->ai_addrlen)==0)
		{
			dbg("Events channel connected\n");
			freeaddrinfo(resfree);
			goto net;
		}

		else
		{
			perror("connecting stream socket");
		}

		close(Xdrcomm->recv_fd); /* ignore */
	}
	while((res=res->ai_next)!= NULL);

	freeaddrinfo(resfree);
	RETRY

net:
	Xdrcomm->xdr_recv.x_op =XDR_DECODE;
	xdrrec_create(&Xdrcomm->xdr_recv, 0, 0, &Xdrcomm->recv_fd, readit, writeit);
	zeroflag =0;

	while(1)
	{
		FD_ZERO(&recvfds);
		FD_SET(Xdrcomm->recv_fd, &recvfds);

		ready = select(Xdrcomm->recv_fd + 1, &recvfds, NULL, NULL, NULL);

		if(ready < 0)
		{
			dbg("Ready < 0\n");

			if(errno == EINTR) continue;  /* interruption, probably signal */
			else break; /* disconnected */
		}
		else if(ready == 0)
		{
			zeroflag++;

			if(zeroflag > 5)
			{
				xdr_destroy(&Xdrcomm->xdr_recv);
				close(Xdrcomm->recv_fd);
				RETRY
			}
		}
		else do
			{
				Event_t *event =calloc(1, sizeof(Event_t));

				if(!xdrrec_skiprecord(&Xdrcomm->xdr_recv))
				{
					dbg("xdr skip record failed\n");
					xdr_destroy(&Xdrcomm->xdr_recv);
					close(Xdrcomm->recv_fd);
					RETRY
				}

				if(!xdr_Event_t(&Xdrcomm->xdr_recv, event))
				{
					dbg("xdr_ToxEvent_t read failed\n");
				}
				else Xdrcomm_despatchevent(event);
			}
			while(!xdrrec_eof(&Xdrcomm->xdr_recv) && ready != 0);
	}

	return 0;
}
