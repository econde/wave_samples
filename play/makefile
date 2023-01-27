all: wave_play

CFLAGS = `pkg-config libwave --cflags` -g -Wall -Werror -pedantic

LIBS = `pkg-config libwave --libs` -lasound -pthread

wave_play: wave_play.o
	gcc wave_play.o $(LIBS) -o wave_play

wave_play.o: wave_play.c
	gcc $(CFLAGS) -c wave_play.c

clean:
	rm -rf *.o *.i
