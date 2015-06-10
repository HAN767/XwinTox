# 
# XwinTox
#

include $(ROOTDIR)/Makefile.cfg

EXE = XwinTox
TARGET = $(EXE)

VERSION = 1

OBJ_C = c/main.c c/etc.c $(ROOTDIR)/libsunshine_src/libsunshine.ar 
OBJ_C += ../toxaemia_src/toxaemia_clnt.c ../toxaemia_src/toxaemia_xdr.c

CFLAGS = -I$(ROOTDIR)/xwintox_src/hdr -I$(ROOTDIR)/libsunshine_src/hdr 
CFLAGS += -I$(ROOTDIR)/toxaemia_src/hdr
CFLAGS += -DXWTOXVERSION=\"$(VERSION)\" -O0 -g 
LDFLAGS = -lstdthreads

build: $(OBJ_C:%.c=%.o) $(EXE)
	@echo Built

include $(ROOTDIR)/Makefile.mk
