CFLAGS_DEBUG =  -g -Wall -fPIC # for Linux and other gcc systems
CFLAGS_RELEASE =  -O2 -Wall -fPIC # for Linux and other gcc systems
OP=$(CFLAGS_DEBUG) -Isweph
CC=cc	#for Linux

# compilation rule for general cases
.o :
	$(CC) $(OP) -o $@ $? -lm
.c.o:
	$(CC) -c $(OP) $<

all:	solarcalc

solarcalc: solarcalc.o
	$(CC) $(OP) -o solarcalc solarcalc.o -Llib/ -lswe -lm -ldl

clean:
	rm -f *.o solarcalc
