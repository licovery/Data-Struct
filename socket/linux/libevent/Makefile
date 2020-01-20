include ../Make.defines

TARGETS = event_server bufferevent_server bufferevent_client
LIBS += -levent

.PHONY: all
all: ${TARGETS}

event_server: event_server.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

bufferevent_server: bufferevent_server.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

bufferevent_client: bufferevent_client.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@
