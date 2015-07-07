PATH.thrcompat := ${.PARSEDIR:tA}/../libsunshine/hdr/compat

MKC_CHECK_HEADERS	+= threads.h
# this is used on FreeBSD 
MKC_CHECK_FUNCLIBS	+= mtx_init:stdthreads

.include <mkc.configure.mk>

# why is this shit necessary
# should i have just stayed with my own make & configure system?
.if !defined(HAVE_HEADER.threads_h})
HAVE_HEADER.threads_h != env ${mkc.environ} mkc_check_header threads.h
.endif

.if !${HAVE_HEADER.threads_h}
CFLAGS	+=	-I${PATH.thrcompat}
LDFLAGS	+=	-lpthreads
.endif
