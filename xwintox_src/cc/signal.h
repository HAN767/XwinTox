#ifndef _SIGNAL_H
#define _SIGNAL_H

#include "list.h"


enum PBMessage
{
	FRequest
};

typedef struct Postbox_s
{
	List_t *objects;
} Postbox_t;

void Register(Postbox_t *pb, 