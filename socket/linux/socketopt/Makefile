include ../Make.defines

TARGETS = client server

.PHONY: all
all: ${TARGETS}

client: client.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

server: server.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@


	
