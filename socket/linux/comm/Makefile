CFLAGS = -g -Wall

.PHONY: all
all: libcomm

libcomm: libcomm.a 

libcomm.a: build *.c *.h 
	@cd build; \
	${CC} ${CFLAGS} -c ../*.c; \
	ar crv ../libcomm.a *.o; \

.PHONY: clean
clean:
	rm -f build/* libcomm.a

build:
	mkdir build

