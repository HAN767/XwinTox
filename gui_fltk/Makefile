# 
# XwinTox
#

LIB 	=	gui_fltk
SRCS	=	GUI_FLTK.cc GUIFLTK.cc
SRCS	+=	control/xwin.cc

SHLIB_MAJOR	=	1

MKPICLIB 	?=	no
MKSTATICLIB	?=	no

PATH.self := ${.PARSEDIR:tA}
CPPFLAGS += -I${PATH.self}

.include "../xwintox/linkme.mk"
.include "../libsunshine/linkme.mk"
.include "../mk/c11threads.mk"
.include "../mk/fltk.mk"

.include <mkc.lib.mk>
