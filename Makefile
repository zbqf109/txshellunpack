SOURCES := unpack.c jiemi.c
HEADERS := unpack.h

unpack.o: unpack.c
	${CC} -c unpack.c

jiemi.o: jiemi.c
	${CC} -c jiemi.c

txshell_unpack: unpack.o jiemi.o
	${CC} unpack.o jiemi.o -o txshell_unpack

all: txshell_unpack

clean:
	rm -rf *.o txshell_unpack

