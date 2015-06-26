# 
# XwinTox
#

include $(ROOTDIR)/Makefile.cfg

EXE = XwinTox
TARGET = $(EXE)

VERSION = 1

OBJ_C = comm/main.c comm/etc.c $(ROOTDIR)/libsunshine_src/libsunshine.ar 
OBJ_C += ../toxaemia_src/toxaemia_clnt.c ../toxaemia_src/toxaemia_xdr.c
OBJ_C += nanosvg/nsvgwrapper.c postbox.c resolv/toxdns.c

OBJ_CXX = xwin/res.cc xwin/cxxmain.cc xwin/contacts.cc xwin/win2.cc control/gui.cc
OBJ_CXX += control/msgarea.cc control/gaddfrnd.cc control/sidebar.cc
OBJ_CXX += control/sbarbtm.cc control/sbartop.cc control/cntctlst.cc
OBJ_CXX += control/cntctent.cc control/svgbox.cc control/statbox.cc
OBJ_CXX += control/garea.cc control/frequest.cc

CFLAGS = -I$(ROOTDIR)/xwintox_src/hdr -I$(ROOTDIR)/libsunshine_src/hdr 
CFLAGS += -I$(ROOTDIR)/toxaemia_src/hdr -I$(ROOTDIR)/xwintox_src
CFLAGS += -DXWTOXVERSION=\"$(VERSION)\" -O0 -g  $(TOXINC)
CXXFLAGS = $(CFLAGS) -I$(ROOTDIR)/xwintox_src/c $(FLTKINC)
LDFLAGS = $(FLTKLIB) $(TOXLIB)

build: $(OBJ_C:%.c=%.o) $(OBJ_CXX:%.cc=%.o) $(EXE)
	@echo Built

include $(ROOTDIR)/Makefile.mk
