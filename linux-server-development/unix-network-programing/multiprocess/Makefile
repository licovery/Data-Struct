include ../Make.defines

TARGETS = call_process echo_server multiprocess 
.PHONY: all
all: ${TARGETS}

call_process: call_process.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

echo_server: echo_server.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

multiprocess: multiprocess.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@





	
