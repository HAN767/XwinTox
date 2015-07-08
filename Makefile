#
# XwinTox
#

LIBDEPS	=	libsunshine:xwintox
LIBDEPS	+=	libsunshine:imcomm_tox

.include <mkc.init.mk>

all: depend

.if !defined(BM)
BM =""
.endif

.if ${BM} == "release"
COPTS += -O
.else
COPTS += -O0 -g
LDFLAGS += -g
.endif
.export COPTS LDFLAGS


.include <mkc.subprj.mk>
