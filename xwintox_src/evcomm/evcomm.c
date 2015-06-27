#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#include "misc.h"
#include "evcomm.h"

int readit(char* handle, char* buf, int len)
{
	int sockfd =*handle;
	int err =recv(sockfd, buf, len, 0);
	dbg("Err: %d, buf: %s", err, buf);
	return err;
}

int writeit(char* handle, char* buf, int len)
{
	int sockfd =*handle;
	int err =send(sockfd, buf, len, 0);
	dbg("Err: %d, buf: %s", err, buf);
	return err;
}

int Evcomm_main(Evcomm_t *evcomm)
{
	int ready =0;
	fd_set recvfds;

	while(1)
	{
		FD_ZERO(&recvfds);
		FD_SET(evcomm->recv_fd, &recvfds);

		ready = select(evcomm->recv_fd + 1, &recvfds, NULL, NULL, NULL);

		if (ready < 0)
		{
			if (errno == EINTR) continue; /* interruption, probably signal */
			else break; /* disconnected */
		}
		else do
		{
			if (!xdrrec_skiprecord(&evcomm->xdr_recv))
			{
				dbg("xdr skip record failed\n");
				break;
			}
			dbg("Process XDRs here\n");
		} 
		while (!xdrrec_eof(&evcomm->xdr_recv));
	}

	return 0;
}
