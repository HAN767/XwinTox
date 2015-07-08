# 
# LibSunshine
#

PROG =			xwintox
VERS =			"DR0"
SRCS.xwintox = 	xwintox.c
SRCS.xwintox+=	module/manager.c

INCSSRCDIR = hdr
INCS = Module/Module.h

PATH.self := ${.PARSEDIR:tA}
CPPFLAGS += -I${PATH.self} -I${PATH.self}/hdr
CPPFLAGS += -I. -DXWVERS=\"${VERS}\"

.include "../mk/c11threads.mk"

.include <mkc.prog.mk>
