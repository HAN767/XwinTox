#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "evserv.h"
#include "misc.h"

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
	dbg("Evserv writeclient err: %d\n", err);

	if(err == 0) err =-1;

	return err;
}

int Evclient_main(void *custom)
{
	Evclient_t *evclient =custom;

	evclient->xdr_write.x_op =XDR_ENCODE;
	xdrrec_create(&evclient->xdr_write, 0, 0, &evclient->fd, readit, writeit);

	while(1)
	{
		Event_t *ev;
		mtx_lock(&evclient->Evlock);

		while(!evclient->Evcount)
		{
			cnd_wait(&evclient->Evcond, &evclient->Evlock);
		}

		if(evclient->Events->List) while((ev =List_retrieve_and_remove_first(
			        evclient->Events)) != 0)
			{
				evclient->xdr_write.x_op =XDR_ENCODE;
				if(xdr_Event_t(&evclient->xdr_write, ev) == FALSE) 
				{ dbg("putting: xdr_Event_t failed.\n"); }
				xdrrec_endofrecord(&evclient->xdr_write, TRUE);
				dbg("Call: %d\n", ev->T, ev->ID);
				Ev_free(ev);
			}

		mtx_unlock(&evclient->Evlock);
	}

	xdr_destroy(&evclient->xdr_write);
	close(evclient->fd);
	return 0;
}

int Evserv_main(void *custom)
{
	Evserv_t *evserv =custom;
	int err;
	struct sockaddr_in server, client;
	char buf[BUFFER_SIZE];

	evserv->fd = socket(AF_INET, SOCK_STREAM, 0);

	if(evserv->fd < 0) dbg("Could not create socket\n");

	server.sin_family = AF_INET;
	server.sin_port = htons(PORT);
	server.sin_addr.s_addr = htonl(INADDR_ANY);

	int opt_val = 1;
	setsockopt(evserv->fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);

	err = bind(evserv->fd, (struct sockaddr *) &server, sizeof(server));

	if(err < 0) dbg("Could not bind socket\n");

	err = listen(evserv->fd, 128);

	if(err < 0) dbg("Could not listen on socket\n");

	dbg("Evserv listening on port %d\n", PORT);

	while(1)
	{
		Evclient_t *evc =calloc(1, sizeof(Evclient_t));
		socklen_t client_len = sizeof(client);

		evc->fd =accept(evserv->fd, (struct sockaddr *) &client, &client_len);

		if(evc->fd < 0)
		{
			dbg("Could not establish new connection\n");
			free(evc);
		}

		evc->Events =List_new();
		mtx_init(&evc->Evlock, mtx_plain);
		cnd_init(&evc->Evcond);

		List_add(evserv->Clients, evc);
		thrd_create(&evc->Thread, Evclient_main, evc);
		dbg("New client to Evserv: %s\n", inet_ntoa(client.sin_addr))
	}

	return 0;
}

Event_t *Ev_copy(Event_t *ev)
{
	return ev;
}

void Ev_pack(Event_t *ev)
{
	if(!ev->S1) ev->S1 =malloc(4);
	if(!ev->S2) ev->S2 =malloc(4);
}

void Ev_free(Event_t *ev)
{
	free(ev->S1);
	free(ev->S2);
	free(ev);
	return;
}

#define EVClientAddWork(CNT) \
	mtx_lock(&c->Evlock); \
	c->Evcount +=CNT ; \
	cnd_broadcast(&c->Evcond); \
	mtx_unlock(&c->Evlock);

void Evserv_send_event(Evserv_t *evserv, Event_t *ev)
{
	LIST_ITERATE_OPEN(evserv->Clients)
	Evclient_t *c =e_data;
	List_add(c->Events, Ev_copy(ev));
	EVClientAddWork(1)
	LIST_ITERATE_CLOSE(evserv->Clients)
	return;
}
