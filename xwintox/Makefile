# 
# LibSunshine
#

PROG =			xwintox
SRCS.xwintox = 	xwintox.c
SRCS.xwintox +=	Module/Manager.c

INCSSRCDIR = hdr
INCS = Module/Module.h

PKG_CONFIG_DEPS= libtoxcore

CPPFLAGS += -I. -I${srcdir}

.include "../mk/c11thread.mk"

.include <mkc.prog.mk>
