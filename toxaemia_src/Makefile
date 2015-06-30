# 
# XwinTox
#

include $(ROOTDIR)/Makefile.cfg

EXE = Toxaemia

RPC = toxaemia.x
rpc_h_loc =hdr/
OBJ_C = toxaemia.c toxaemia_svc.c $(ROOTDIR)/libsunshine_src/libsunshine.ar
OBJ_C += toxaemia_xdr.c toxaemia_funcs.c toxaemia_tox_callbacks.c
OBJ_C += xdrserv/xdrserv.c xdrserv/evserv.c xdrserv/fileserv.c

CFLAGS = -I$(ROOTDIR)/libsunshine_src/hdr -O0 -g -Ihdr -DRPC_SVC_FG
CFLAGS += -DXWTOXVERSION=\"$(VERSION)\" $(TOXINC) -Wno-unused-variable -I.
LDFLAGS = $(TOXLIB)

build: $(RPC:%.x=hdr/%_rpc.h) $(RPC:%.x=%_svc.c) $(RPC:%.x=%_xdr.c) \
  $(RPC:%.x=%_clnt.c) $(OBJ_C:%.c=%.o) $(EXE)
	@echo Built

include $(ROOTDIR)/Makefile.mk
