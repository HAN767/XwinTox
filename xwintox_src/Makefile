# 
# XwinTox
#

include $(ROOTDIR)/Makefile.cfg

EXE = XwinTox
TARGET = $(EXE)

VERSION = 1

OBJ_C = c/main.c c/etc.c $(ROOTDIR)/libsunshine_src/libsunshine.ar 
OBJ_C += ../toxaemia_src/toxaemia_clnt.c ../toxaemia_src/toxaemia_xdr.c
OBJ_C += nanosvg/nsvgwrapper.c

OBJ_CXX = cc/win.cc cc/cxxmain.cc cc/contacts.cc cc/win2.cc

CFLAGS = -I$(ROOTDIR)/xwintox_src/hdr -I$(ROOTDIR)/libsunshine_src/hdr 
CFLAGS += -I$(ROOTDIR)/toxaemia_src/hdr -I$(ROOTDIR)/xwintox_src
CFLAGS += -DXWTOXVERSION=\"$(VERSION)\" -O0 -g 
CXXFLAGS = $(CFLAGS) -I$(ROOTDIR)/xwintox_src/c $(FLTKINC)
LDFLAGS = -lstdthreads $(FLTKLIB)

build: $(OBJ_C:%.c=%.o) $(OBJ_CXX:%.cc=%.o) $(EXE)
	@echo Built

include $(ROOTDIR)/Makefile.mk
