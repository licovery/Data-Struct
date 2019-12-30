include ../Make.defines

TARGETS = tcp_client tcp_server

.PHONY: all
all: ${TARGETS}

tcp_client: tcp_client.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

tcp_server: tcp_server.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@


	
