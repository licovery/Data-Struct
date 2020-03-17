include ../Make.defines

TARGETS = nonblock_connect 

.PHONY: all
all: ${TARGETS}

nonblock_connect: nonblock_connect.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@



	
