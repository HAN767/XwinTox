#ifndef GTRNSFER_H_
#define GTRNSFER_H_

#include <time.h>

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

typedef enum TransferDir_e
{
	TR_Recv,
	TR_Send
} TransferDir_e;

typedef struct Transfer_s
{
	TransferState_e state;
	TransferDir_e dir; /* dir = 0 means receive, 1 means send */
	char *filename;
	Contact_t *contact;
	unsigned int num, size, pos, donebytes;
	struct tm *time;
	FILE* file;
} Transfer_t;

class TransfersList;

class GTransfers: public GArea
{
public:
	// Ctor
	GTransfers(int S);

	void resize(int X, int Y, int W, int H);
	void draw();

	void regen_gui();

	TransfersList *list;
	vector <Transfer_t*> transfers;
	int regengui;
};

#endif
