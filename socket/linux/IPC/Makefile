include ../Make.defines

TARGETS = pipe semaphore

.PHONY: all
all: ${TARGETS}

pipe: pipe.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

semaphore: semaphore.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

