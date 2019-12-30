include ../Make.defines

TARGETS = nonblock_connect io_timeout

.PHONY: all
all: ${TARGETS}

nonblock_connect: nonblock_connect.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

io_timeout: io_timeout.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@


	
