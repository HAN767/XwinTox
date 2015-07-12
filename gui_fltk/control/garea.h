#ifndef GAREA_H_
#define GAREA_H_

class GArea: public Fl_Group
{
public:
	// Ctor
	GArea(const XWF_hObj_t* hObj, int S, const char *C);

	void draw();

	const char *caption;
	int scale;
	const XWF_hObj_t* hObj_;
};

#endif
