# 
# XwinTox
#

include $(ROOTDIR)/Makefile.cfg

EXE = Toxaemia

RPC = toxaemia.x
rpc_h_loc =hdr/
OBJ_C = main.c $(ROOTDIR)/libsunshine_src/libsunshine.ar

CFLAGS = -I$(ROOTDIR)/libsunshine_src/hdr -O0 -g -Ihdr
CFLAGS += -DXWTOXVERSION=\"$(VERSION)\"
LDFLAGS = -lstdthreads

build: $(RPC:%.x=hdr/%_rpc.h) $(OBJ_C:%.c=%.o) $(EXE)
	@echo Built

include $(ROOTDIR)/Makefile.mk
