# 
# LibSunshine
#

PROG =			xwintox
VERS =			"DR0"
SRCS.xwintox = 	xwintox.c
SRCS.xwintox+=	Module/Manager.c

INCSSRCDIR = hdr
INCS = Module/Module.h

PKG_CONFIG_DEPS= libtoxcore

CPPFLAGS += -I. -DXWVERS=\"${VERS}\"

.include "../mk/c11thread.mk"

.include <mkc.prog.mk>
