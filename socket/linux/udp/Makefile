include ../Make.defines

TARGETS = client server select_tcp_udp_server.c

.PHONY: all
all: ${TARGETS}

client: client.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

server: server.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

select_tcp_udp_server: select_tcp_udp_server.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@



	
