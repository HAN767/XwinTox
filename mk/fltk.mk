NOCOLOUR=\033[0m
RED=\033[0;31m
REDBOLD=\033[31;01m

FLTK_CONFIG		?= fltk-config
MKC_CHECK_PROGS += $(FLTK_CONFIG)

.include <mkc.configure.mk>

.if !defined(HAVE_PROG.${FLTK_CONFIG})
HAVE_PROG.$(FLTK_CONFIG) != env ${mkc.environ} mkc_check_prog $(FLTK_CONFIG)
.endif

.if !${HAVE_PROG.${FLTK_CONFIG}}
	@printf "$(RED) fltk-config not found.\n"
	@printf "$(REDBOLD) This build will likely fail.$(NOCOLOUR)\n"
.else
FLTK_CFLAGS 	!= $(FLTK_CONFIG) --cxxflags
FLTK_LDFLAGS 	!= $(FLTK_CONFIG) --ldstaticflags
CFLAGS 			+= $(FLTK_CFLAGS)
LDFLAGS 		+= $(FLTK_LDFLAGS)
.endif
