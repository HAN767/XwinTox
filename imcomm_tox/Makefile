# 
# XwinTox
#

LIB 	=	imcomm_tox
SRCS	=	imcomm_tox.c IMCommTox.c

SHLIB_MAJOR	=	1

MKPICLIB 	?=	no
MKSTATICLIB	?=	no

PKG_CONFIG_DEPS= libtoxcore

.include "../xwintox/linkme.mk"
.include "../libsunshine/linkme.mk"
.include "../mk/c11threads.mk"

.include <mkc.lib.mk>
