include ../Make.defines

TARGETS = getaddr gethost

.PHONY: all
all: ${TARGETS}

getaddr: getaddr.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

gethost: gethost.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@


	
