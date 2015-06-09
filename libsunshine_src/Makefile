# 
# LibSunshine
#

include $(ROOTDIR)/Makefile.cfg

STLIB=libsunshine.ar
OBJ_C= list.c misc.c dictionary.c ini/ini.c

CFLAGS = -I$(ROOTDIR)/libsunshine_src/hdr -O0 -g -fPIC

build: $(OBJ_C:%.c=%.o) $(STLIB)
	@echo Built

include $(ROOTDIR)/Makefile.mk
