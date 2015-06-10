# 
# SunshinePro D-Make
#

NOCOLOUR=\033[0m
GREENBOLD=\033[1;32m
REDBOLD=\033[31;01m
BLUE=\033[0;34m
BLUEBOLD=\033[1;34m
BUILTTARGET=printf " $(BLUE)built target $(BLUEBOLD)$(@)$(NOCOLOUR)\n"
OK=printf " $(GREENBOLD)[OK]$(NOCOLOUR)\n"

default: all

all: build
	@echo Complete.

.c.o: $(RPC)
	@set -e; rm -f $@; \
	printf " DEP \t\t$(@)" 
	@$(CC) -MM -MT"$*.o" $(BASECFLAGS) $(CFLAGS) $(WARN) $(CPPFLAGS) $(XFLAGS) $< > $@.dddd; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.dddd > $*.d; \
	rm -f $@.dddd; \
	touch autodeps.mk; \
	( grep "$(*).d" autodeps.mk > /dev/null \
	|| echo "-include $(*).d" >> autodeps.mk ) \
	&& $(OK)
	@printf " cc \t\t$(@)" 
	@$(CC) $(CFLAGS) $(WARN) $(CPPFLAGS) $(XFLAGS) -c $< -o $@ && $(OK)
	$(ANALYSE)@printf " ccSA \t\t$(@)" 
	$(ANALYSE)@$(CC) $(CFLAGS) $(WARN) $(CPPFLAGS) $(XFLAGS) -c $< --analyze -Xanalyzer -analyzer-output=html -o $(ROOTDIR)/analysis && $(OK)

.cc.o: $(RPC)
	@set -e; rm -f $@; \
	printf " DEP \t\t$(@)" 
	@$(CXX) -MM -MT"$*.o" $(BASECFLAGS) $(CXXFLAGS) $(WARN) $(CPPFLAGS) $(XFLAGS) $< > $@.dddd; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.dddd > $*.d; \
	rm -f $@.dddd; \
	touch autodeps.mk; \
	( grep "$(*).d" autodeps.mk > /dev/null \
	|| echo "-include $(*).d" >> autodeps.mk ) \
	&& $(OK)
	@printf " CC \t\t$(@)" 
	@$(CXX) $(CXXFLAGS) $(WARN) $(CPPFLAGS) $(XFLAGS) -c $< -o $@ && $(OK)
	$(ANALYSE)@printf " CCSA \t\t$(@)" 
	$(ANALYSE)@$(CXX) $(CXXFLAGS) $(WARN) $(CPPFLAGS) $(XFLAGS) -c $< --analyze -Xanalyzer -analyzer-output=html -o $(ROOTDIR)/analysis && $(OK)

$(rpc_h_loc)%_rpc.h: %.x
	@printf " RPCGEN(H) \t$(@)"
	@rpcgen -C -N -h $< > $@ && $(OK)

%_svc.c: %.x
	@printf " RPCGEN(SVC) \t$(@)"
	@rpcgen -C -N -s udp $< > $@.pre && \
	sed  's/.h"/_rpc.h"/' $(@).pre > $(@) && rm $(@).pre && $(OK)

%_xdr.c: %.x
	@printf " RPCGEN(XDR) \t$(@)"
	@rpcgen -C -N -c $< > $@.pre && \
	sed  's/.h"/_rpc.h"/' $(@).pre > $(@) && rm $(@).pre && $(OK)

%_clnt.c: %.x
	@printf " RPCGEN(CLNT) \t$(@)"
	@rpcgen -C -N -l $< > $@.pre && \
	sed  's/.h"/_rpc.h"/' $(@).pre > $(@) && rm $(@).pre && $(OK)

$(EXE): $(OBJ_C:%.c=%.o) $(OBJ_CXX:%.cc=%.o)
	@set -e; \
	printf " LD \t\t$(@)\n"
	@cc $(OBJ_C:%.c=%.o) $(OBJ_CXX:%.cc=%.o) $(LDFLAGS) $(XFLAGS) $(LDFLAGS) -o $(EXE) && $(BUILTTARGET)

$(SHLIB): $(OBJ_C:%.c=%.o)
	@set -e; \
	printf " LD(SHLIB) \t\t$(@)\n"
	@cc $(OBJ:%.c=%.o) $(LDFLAGS) $(XFLAGS) $(LDFLAGS) -shared -o $(SHLIB) && $(BUILTTARGET)

$(STLIB): $(OBJ_C:%.c=%.o)
	@set -e; \
	printf " AR \t\t$(@)\n"
	@ar rcs $(STLIB) $(OBJ_C:%.c=%.o) $(XFLAGS) && $(BUILTTARGET)

clean_subdir:
	rm -f $(OBJ_C:%.c=%.o) $(OBJ_CXX:%.cc=%.o) $(OBJ_C:%.c=%.d) autodeps.mk $(EXE) 
	rm -f $(SHLIB) $(STLIB) $(rpc_h_loc)$(RPC:%.x=%_rpc.h) $(RPC:%.x=%_svc.c)
	rm -f $(RPC:%.x=%_xdr.c) $(RPC:%.x=%_clnt.c) $(OBJ_CXX:%.cc=%.d)

-include autodeps.mk
