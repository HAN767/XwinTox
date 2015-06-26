#include <threads.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>

#include <tox/tox.h>
#include <tox/toxdns.h>

#include "hexstring.h"
#include "misc.h"
#include "list.h"

#include "xwintox.h"
#include "dresolv.h"

Tox3_t Tox3Server[] =
{
	{
		.name ="utox.org",
		.key ={
			0xD3, 0x15, 0x4F, 0x65, 0xD2, 0x8A, 0x5B, 0x41, 0xA0, 0x5D, 0x4A, 0xC7, 0xE4, 0xB3, 0x9C, 0x6B,
			0x1C, 0x23, 0x3C, 0xC8, 0x57, 0xFB, 0x36, 0x5C, 0x56, 0xE8, 0x39, 0x27, 0x37, 0x46, 0x2A, 0x12
		}
	},
	{
		.name ="toxme.se",
		.key ={
			0x5D, 0x72, 0xC5, 0x17, 0xDF, 0x6A, 0xEC, 0x54, 0xF1, 0xE9, 0x77, 0xA6, 0xB6, 0xF2, 0x59, 0x14,
			0xEA, 0x4C, 0xF7, 0x27, 0x7A, 0x85, 0x02, 0x7C, 0xD9, 0xF5, 0x19, 0x6D, 0xF1, 0x7E, 0x0B, 0x13
		}
	}
};

short Tox3Server_cnt =2;

void Resolv_DNSResolve(char* address)
{
	int server =-1, out_len, response_len, rrmax, rd_len, rd_id_offset;
	char *name, *domain =address, newdomain[255], txt[1024];
	const char *rd;
	unsigned char response[PACKETSZ];
	void *toxdns3;
	uint8_t out[256], tox_id[TOX_ADDRESS_SIZE];
	uint32_t request_id;
	ns_msg msg;
	ns_rr rr;

	name =strsep(&domain, "@");
	dbg("Name: %s. Domain: %s\n", name, domain);

	for(int i =0; i < Tox3Server_cnt; i++)
	{
		if(strcmp(Tox3Server[i].name, domain) == 0) server =i;
	}

	dbg("Server: %d\n", server);

	if(server == -1) goto fail;

	toxdns3 =tox_dns3_new(Tox3Server[server].key);
	out_len =tox_generate_dns3_string(toxdns3, out, sizeof(out), &request_id,
	                                  (uint8_t*)name, strlen(name));

	if(out_len < 0) goto fail;

	out[out_len] ='\0';

	snprintf(newdomain, sizeof(newdomain), "_%s._tox.%s", out, domain);

	dbg("Domain to query: %s\n", newdomain);


	if((response_len
	        =res_query(newdomain, C_IN, T_TXT, response, sizeof(response))) <=0)
	{
		goto fail;
	}

	if(ns_initparse(response, response_len, &msg) < 0) goto fail;

	rrmax =ns_msg_count(msg, ns_s_an);

	if(ns_parserr(&msg, ns_s_an, 0, &rr)) goto fail;

	rd =(const char*)ns_rr_rdata(rr);

	rd_len =rd[0];

	strncpy((char*)&txt, rd + 1, rd_len);
	txt[rd_len] ='\0';

	dbg("Response: %s\n", txt);

	for(rd_id_offset = rd_len - 1; rd_id_offset != 0 && rd[rd_id_offset] != '=';
	        --rd_id_offset);

	if(tox_decrypt_dns3_TXT(toxdns3, (uint8_t*)tox_id,
	                        (uint8_t*)txt + rd_id_offset,
	                        strlen(txt) - (rd_id_offset), request_id) != 0)
	{
		dbg("Failed");
		goto fail;
	}
	else
	{
		char *toxid =bin_to_hex_string(tox_id, TOX_ADDRESS_SIZE);
		dbg("Tox ID: %s\n", toxid);
		free (toxid);
		return;
	}

fail:
	dbg("Fail\n");
}

void Resolv_main()
{
	/* now wait until we are needed */
	while(1)
	{
		Call_t *call;

		mtx_lock(&APP->Resolv->CallsMtx);

		while(!APP->Resolv->CallsCnt)
		{
			cnd_wait(&APP->Resolv->CallsCnd, &APP->Resolv->CallsMtx);
		}

		mtx_unlock(&APP->Resolv->CallsMtx);

		if(APP->Resolv->Calls->List) while((call =List_retrieve_and_remove_first(
			                                        APP->Resolv->Calls)) != 0)
			{
				dbg("Call: %d\n", call->Func);

				if(call->Func == R_DNSResolve) Resolv_DNSResolve(call->S1);

				mtx_lock(&APP->Resolv->CallsMtx);
				APP->Resolv->CallsCnt -=1;
				mtx_unlock(&APP->Resolv->CallsMtx);
			}
	}
}
