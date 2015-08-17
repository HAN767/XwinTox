#include <arpa/nameser.h>
#include <cstdio>
#include <cstring>
#include <netinet/in.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <resolv.h>
#include <threads.h>
#include <unistd.h>

#include <tox/toxdns.h>

#include "misc.h"
#include "hexstring.h"

#include "xwintox.h"

#include "AOM/IXWClass.h"
#include "Module/Module.h"
#include "TOXDNS3.h"

const TOXDNS3::Tox3_t TOXDNS3::Tox3Servers[]{
    {"toxme.se", (uint8_t[32]){0x5D, 0x72, 0xC5, 0x17, 0xDF, 0x6A, 0xEC, 0x54,
                               0xF1, 0xE9, 0x77, 0xA6, 0xB6, 0xF2, 0x59, 0x14,
                               0xEA, 0x4C, 0xF7, 0x27, 0x7A, 0x85, 0x02, 0x7C,
                               0xD9, 0xF5, 0x19, 0x6D, 0xF1, 0x7E, 0x0B, 0x13}},
    {"utox.org", (uint8_t[32]){0xD3, 0x15, 0x4F, 0x65, 0xD2, 0x8A, 0x5B, 0x41,
                               0xA0, 0x5D, 0x4A, 0xC7, 0xE4, 0xB3, 0x9C, 0x6B,
                               0x1C, 0x23, 0x3C, 0xC8, 0x57, 0xFB, 0x36, 0x5C,
                               0x56, 0xE8, 0x39, 0x27, 0x37, 0x46, 0x2A, 0x12}},
    {"sr.ht", (uint8_t[32]){0x05, 0x08, 0x88, 0xFE, 0x0C, 0x88, 0x29, 0xAA,
                            0x43, 0xE0, 0xC9, 0xFD, 0x49, 0x6D, 0x6C, 0x4A,
                            0xFE, 0xEF, 0xEB, 0x6D, 0x55, 0x71, 0xF4, 0x2F,
                            0x30, 0xEB, 0x5F, 0x3B, 0x4C, 0xD9, 0x4F, 0xE5}}};

TOXDNS3::TOXDNS3 (XWF_ObjectParams_t * prmObj) : Tox3Server_cnt (3)
{
    dbg ("New ToxDNS3 created\n");
}

int TOXDNS3::start ()
{
    xwfRegisterFilter (frSendRequest);

    return 0;
}

void TOXDNS3::filterSignal (unsigned int, PBMessage_t * msg)
{
    int server = -1, out_len, response_len, rrmax, rd_len, rd_id_offset;
    char * name, * domain = msg->S1, * tofree = msg->S1, newdomain[255],
                   txt[1024];
    const char * rd;
    unsigned char response[PACKETSZ];
    void * toxdns3;
    uint8_t out[256], tox_id[TOX_ADDRESS_SIZE];
    uint32_t request_id;
    ns_msg nsmsg;
    ns_rr rr;

    if (!strchr (msg->S1, '@'))
    {
        return;
    }

    name = strsep (&domain, "@");
    dbg ("Name: %s. Domain: %s\n", name, domain);

    for (int i = 0; i < Tox3Server_cnt; i++)
    {
        if (strcmp (Tox3Servers[i].name, domain) == 0)
            server = i;
    }

    dbg ("Server: %d\n", server);

    if (server == -1)
        goto fail;

    toxdns3 = tox_dns3_new (Tox3Servers[server].key);
    out_len = tox_generate_dns3_string (toxdns3, out, sizeof (out), &request_id,
                                        (uint8_t *)name, strlen (name));

    if (out_len < 0)
        goto fail;

    out[out_len] = '\0';

    snprintf (newdomain, sizeof (newdomain), "_%s._tox.%s", out, domain);

    dbg ("Domain to query: %s\n", newdomain);

    if ((response_len = res_query (newdomain, C_IN, T_TXT, response,
                                   sizeof (response))) <= 0)
    {
        goto fail;
    }

    if (ns_initparse (response, response_len, &nsmsg) < 0)
        goto fail;

    rrmax = ns_msg_count (nsmsg, ns_s_an);

    if (ns_parserr (&nsmsg, ns_s_an, 0, &rr))
        goto fail;

    rd = (const char *)ns_rr_rdata (rr);

    rd_len = rd[0];

    strncpy ((char *)&txt, rd + 1, rd_len);
    txt[rd_len] = '\0';

    dbg ("Response: %s\n", txt);

    for (rd_id_offset = rd_len - 1;
         rd_id_offset != 0 && rd[rd_id_offset] != '='; --rd_id_offset)
        ;

    if (tox_decrypt_dns3_TXT (toxdns3, (uint8_t *)tox_id,
                              (uint8_t *)txt + rd_id_offset,
                              strlen (txt) - (rd_id_offset), request_id) != 0)
    {
        dbg ("Failed to decrypt\n");
    }
    else
    {
        char * toxid = bin_to_hex_string (tox_id, TOX_ADDRESS_SIZE);
        msg->S1 = toxid;
        dbg ("Tox ID: %s\n", toxid);
        free (tofree);
    }
    return;

fail:
    dbg ("Failure\n");
}
