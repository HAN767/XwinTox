# 
# LibSunshine
#

LIB 	=	sunshine
SRCS	=	dictionary.c hexstring.c list.c misc.c ini/ini.c

INCS		=	dictionary.h hexstring.h list.h misc.h ini/ini.h
INCSSRCDIR	=	hdr

MKPIE = yes

PATH.self := ${.PARSEDIR:tA}
CPPFLAGS			+= -I${PATH.self} -I${PATH.self}/hdr

.include <mkc.lib.mk>
