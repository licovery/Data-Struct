include ../Make.defines

TARGETS = pipe

.PHONY: all
all: ${TARGETS}

pipe: pipe.c libcomm
	${CC} ${CFLAGS} ${LIBPATHS} $@.c ${LIBS} ${ODIR}/$@

