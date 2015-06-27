#include <errno.h>
#include <netdb.h>
#include <stdio.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <unistd.h>

#include "misc.h"
#include "evcomm.h"

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

int Evcomm_main(void *data)
{
	int ready =0, err, zeroflag;
	struct addrinfo hints, *res, *resfree;
	char *remote ="localhost", *port ="5554";
	fd_set recvfds;
	Evcomm_t *evcomm =data;

reconnect:
	bzero(&hints, sizeof(struct addrinfo));
	hints.ai_family=AF_UNSPEC;
	hints.ai_socktype=SOCK_STREAM;
	hints.ai_protocol=IPPROTO_TCP;

	if((err =getaddrinfo(remote, port, &hints, &res)) != 0)
	{
		dbg("getaddrinfo error for %s:%s; %s", remote, port, gai_strerror(err));
		RETRY
	}

	resfree=res;

	do
	{
		evcomm->recv_fd =socket(res->ai_family, res->ai_socktype, res->ai_protocol);

		if(evcomm->recv_fd < 0)
			continue;  /*ignore Ip addr*/

		if(connect(evcomm->recv_fd, res->ai_addr, res->ai_addrlen)==0)
		{
			dbg("Events channel connected\n");
			freeaddrinfo(resfree);
			goto net;
		}

		else
		{
			perror("connecting stream socket");
		}

		close(evcomm->recv_fd); /* ignore */
	}
	while((res=res->ai_next)!= NULL);

	freeaddrinfo(resfree);
	RETRY

net:

	evcomm->xdr_recv.x_op =XDR_DECODE;
	xdrrec_create(&evcomm->xdr_recv, 0, 0, &evcomm->recv_fd, readit, writeit);
	zeroflag =0;

	while(1)
	{
		FD_ZERO(&recvfds);
		FD_SET(evcomm->recv_fd, &recvfds);

		ready = select(evcomm->recv_fd + 1, &recvfds, NULL, NULL, NULL);

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
				xdr_destroy(&evcomm->xdr_recv);
				close(evcomm->recv_fd);
				RETRY
			}
		}
		else do
			{
				int val;

				if(!xdrrec_skiprecord(&evcomm->xdr_recv))
				{
					dbg("xdr skip record failed\n");
					xdr_destroy(&evcomm->xdr_recv);
					close(evcomm->recv_fd);
					RETRY
				}

				if(!xdr_int(&evcomm->xdr_recv,&val))
				{
					dbg("xdr_int read failed\n");
				}
			}
			while(!xdrrec_eof(&evcomm->xdr_recv) && ready != 0);
	}

	return 0;
}
