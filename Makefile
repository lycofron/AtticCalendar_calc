CFLAGS_DEBUG =  -g -Wall -fPIC # for Linux and other gcc systems
CFLAGS_RELEASE =  -O2 -Wall -fPIC # for Linux and other gcc systems
OP=$(CFLAGS_DEBUG) -Isweph
CC=cc	#for Linux

# compilation rule for general cases
.o :
	$(CC) $(OP) -o $@ $? -lm
.c.o:
	$(CC) -c $(OP) $<

all:	solarcalc lunarcalc sunrise_sunset_calc

solarcalc: solarcalc.o
	$(CC) $(OP) -O3 -o solarcalc solarcalc.c common.c athens.c -Llib/ -lswe -lm -ldl

lunarcalc: lunarcalc.o
	$(CC) $(OP) -O3 -o lunarcalc lunarcalc.c common.c athens.c -Llib/ -lswe -lm -ldl

sunrise_sunset_calc: sunrise_sunset_calc.o
		$(CC) $(OP) -O3 -o sunrise_sunset_calc sunrise_sunset_calc.c common.c athens.c -Llib/ -lswe -lm -ldl

clean:
	rm -f *.o solarcalc lunarcalc
