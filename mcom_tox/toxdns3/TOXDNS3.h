#ifndef __TOXFLTK__H
#define __TOXFLTK__H

#include <string>

#include <threads.h>
#include <tox/tox.h>

#include "misc.h"
#include "dictionary.h"

#include "Module/Module.h"
#include "AOM/IXWClass.h"

class TOXDNS3 : public XWClassT< TOXDNS3 >
{
  public:
    TOXDNS3 (XWF_ObjectParams_t * pobjParams);
    int start ();
    void filterSignal (unsigned int, PBMessage_t *);

  private:
    typedef struct Tox3_s
    {
        Tox3_s () = default;
        Tox3_s (const char * _name, uint8_t _pk[32]) : name{_name}, key{_pk} {}

        const char * name;
        uint8_t * key;
    } Tox3_t;
    static const Tox3_t Tox3Servers[];
    int Tox3Server_cnt;
};

#endif
