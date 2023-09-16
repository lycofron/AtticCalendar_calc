CFLAGS_DEBUG =  -g -Wall -fPIC # for Linux and other gcc systems
CFLAGS_RELEASE =  -O2 -Wall -fPIC # for Linux and other gcc systems
OP=$(CFLAGS_DEBUG) -Isweph
CC=cc	#for Linux

# compilation rule for general cases
.o :
	$(CC) $(OP) -o $@ $? -lm
.c.o:
	$(CC) -c $(OP) $<

all:	solarcalc lunarcalc

solarcalc: solarcalc.o
	$(CC) $(OP) -o solarcalc solarcalc.c common.c athens.c -Llib/ -lswe -lm -ldl

lunarcalc: lunarcalc.o
	$(CC) $(OP) -o lunarcalc lunarcalc.c common.c athens.c -Llib/ -lswe -lm -ldl

clean:
	rm -f *.o solarcalc
