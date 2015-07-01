#ifndef GTRNSFER_H_
#define GTRNSFER_H_

#include <FL/Fl_Input.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>

#include "control/garea.h"
#include "control/translst.h"

typedef enum TransferState_e
{
    TR_Waiting,
    TR_Active,
    TR_Pause
} TransferState_e;

typedef struct Transfer_s
{
	TransferState_e state;
	char *filename;
	Contact_t *contact;
	int num, size, pos, dir; /* dir = 0 means receive, 1 means send */
} Transfer_t;


class GTransfers: public GArea
{
public:
	// Ctor
	GTransfers(int S);

	void resize(int X, int Y, int W, int H);
	void draw();

	TransfersList *list;
	vector <Transfer_t*> transfers;
};

#endif
