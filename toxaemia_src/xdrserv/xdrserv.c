#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "xdrserv.h"
#include "misc.h"

int Xdr_readit(void* handle, void* buf, int len)
{
	int sockfd =*(int*)handle;
	int err =recv(sockfd, buf, len, 0);
	dbg("Xdrserv read err: %d\n", err);

	if(err == 0) err =-1;

	return err;
}

int Xdr_writeit(void* handle, void* buf, int len)
{
	int sockfd =*(int*)handle;
	int err =send(sockfd, buf, len, 0);
	dbg("Xdrserv writeclient err: %d\n", err);

	if(err == 0) err =-1;

	return err;
}


int Xdrserv_main(void *custom)
{
	Xdrserv_t *Xdrserv =custom;
	int err;
	struct sockaddr_in server, client;

	Xdrserv->fd = socket(AF_INET, SOCK_STREAM, 0);

	if(Xdrserv->fd < 0) dbg("Could not create socket\n");

	server.sin_family = AF_INET;
	server.sin_port = htons(Xdrserv->port);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	int opt_val = 1;
	setsockopt(Xdrserv->fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);

	err = bind(Xdrserv->fd, (struct sockaddr *) &server, sizeof(server));

	if(err < 0) dbg("Could not bind socket\n");

	err = listen(Xdrserv->fd, 128);

	if(err < 0) dbg("Could not listen on socket\n");

	dbg("Xdrserv listening on port %d\n", Xdrserv->port);

	while(1)
	{
		Xdrclient_t *evc =calloc(1, sizeof(Xdrclient_t));
		socklen_t client_len = sizeof(client);

		evc->fd =accept(Xdrserv->fd, (struct sockaddr *) &client, &client_len);

		if(evc->fd < 0)
		{
			dbg("Could not establish new connection\n");
			free(evc);
		}

		evc->Events =List_new();
		evc->ops =Xdrserv->ops;
		mtx_init(&evc->Evlock, mtx_plain);
		cnd_init(&evc->Evcond);

		List_add(Xdrserv->Clients, evc);
		thrd_create(&evc->Thread, Xdrserv->ops->clmain, evc);
		dbg("New client to Xdrserv: %s\n", inet_ntoa(client.sin_addr))
	}

	return 0;
}

#define XdrclientAddWork(CNT) \
	mtx_lock(&c->Evlock); \
	c->Evcount +=CNT ; \
	cnd_broadcast(&c->Evcond); \
	mtx_unlock(&c->Evlock);

void Xdrserv_send(Xdrserv_t *Xdrserv, void *ev)
{
	LIST_ITERATE_OPEN(Xdrserv->Clients)
	Xdrclient_t *c =e_data;
	List_add(c->Events, Xdrserv->ops->copy(ev));
	XdrclientAddWork(1)
	LIST_ITERATE_CLOSE(Xdrserv->Clients)
	return;
}
