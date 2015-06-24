#ifndef FREQUEST_H_
#define FREQUEST_H_

#include <vector>
using namespace std;

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Multiline_Output.H>

extern "C"
{
#include "xwintox.h"
}

const int frheight=112;

class FriendRequests: public Fl_Box
{
public:
	// Ctor
	FriendRequests(int X, int Y, int S);

	void show();
	void hide();
	/*int handle(int event);*/

	Fl_Multiline_Output *message;
	Fl_Button *accept, *reject;

	vector <FriendRequest_t*> frs;
	int selected, scale;
};

#endif
