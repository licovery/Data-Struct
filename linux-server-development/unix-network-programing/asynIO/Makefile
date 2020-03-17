include ../Make.defines

TARGETS = time_client_asio time_server_asio

.PHONY: all
all: ${TARGETS}

time_client_asio: time_client_asio.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

time_server_asio: time_server_asio.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@




	
