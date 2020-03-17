include ../Make.defines

TARGETS =   select_client \
            select_server \
            poll_server \
            epoll_server \

.PHONY: all
all: ${TARGETS}

select_client: select_client.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

select_server: select_server.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c echo.c ${LIBS} ${ODIR}/$@

poll_server: poll_server.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c echo.c ${LIBS} ${ODIR}/$@

epoll_server: epoll_server.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c echo.c ${LIBS} ${ODIR}/$@


	
