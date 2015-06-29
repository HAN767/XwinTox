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
	dbg("Err: %d\n", err);

	if(err == 0) err =-1;

	return err;
}

int Evclient_main(void *custom)
{
	Evclient_t *evclient =custom;

	evclient->xdr_write.x_op =XDR_DECODE;
	xdrrec_create(&evclient->xdr_write, 0, 0, &evclient->fd, readit, writeit);

	while (1) { sleep(1); }

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

		evc->Messages =List_new();

		List_add(evserv->Clients, evc);
		thrd_create(&evc->Thread, Evclient_main, evc);
		dbg("New client to Evserv: %s\n", inet_ntoa(client.sin_addr))
	}

	return 0;
}
