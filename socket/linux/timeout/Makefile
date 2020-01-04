include ../Make.defines

TARGETS =  alarm_select_timeout setopt_accept_timeout setopt_connect_timeout

.PHONY: all
all: ${TARGETS}


alarm_select_timeout: alarm_select_timeout.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

setopt_accept_timeout: setopt_accept_timeout.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

setopt_connect_timeout: setopt_connect_timeout.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@


	
