# 
# XwinTox
#

include $(ROOTDIR)/Makefile.cfg

EXE = XwinTox
TARGET = $(EXE)

VERSION = 1

OBJ_C = c/main.c c/etc.c $(ROOTDIR)/libsunshine_src/libsunshine.ar

CFLAGS = -I$(ROOTDIR)/xwintox_src/hdr -I$(ROOTDIR)/libsunshine_src/hdr -O0 -g 
CFLAGS += -DXWTOXVERSION=\"$(VERSION)\"
LDFLAGS = -lstdthreads

build: $(OBJ_C:%.c=%.o) $(EXE)
	@echo Built

include $(ROOTDIR)/Makefile.mk
