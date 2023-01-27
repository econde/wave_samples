all: wave_dump

CFLAGS = `pkg-config libwave --cflags` -g -Wall -Werror -pedantic

LIBS = `pkg-config libwave --libs`

wave_dump: wave_dump.o
	gcc wave_dump.o $(LIBS) -o wave_dump

wave_dump.o: wave_dump.c
	gcc $(CFLAGS) -c wave_dump.c

clean:
	rm -rf *.o *.i
