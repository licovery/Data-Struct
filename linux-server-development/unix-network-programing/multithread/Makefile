include ../Make.defines

TARGETS =  multithread echo_client echo_server \
			thread_spec_data concurrent_write_error pthread_join_anythread

.PHONY: all
all: ${TARGETS}

multithread: multithread.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

echo_client: echo_client.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

echo_server: echo_server.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

thread_spec_data: thread_spec_data.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

concurrent_write_error: concurrent_write_error.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

pthread_join_anythread: pthread_join_anythread.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@



	
