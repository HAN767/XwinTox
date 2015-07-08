# 
# LibSunshine
#

LIB 	=	sunshine
SRCS	=	dictionary.c hexstring.c list.c misc.c postbox.c ini/ini.c

INCS		=	dictionary.h hexstring.h list.h misc.h postbox.h ini/ini.h
INCSSRCDIR	=	hdr

MKPIE = yes

PATH.self := ${.PARSEDIR:tA}
CPPFLAGS			+= -I${PATH.self} -I${PATH.self}/hdr

.include <mkc.lib.mk>
