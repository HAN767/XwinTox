#include <stdio.h>
#include <stdlib.h>

#include "evserv.h"
#include "misc.h"

int Evclient_main(void *custom)
{
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

	}

	return 0;
}
