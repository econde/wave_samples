all: wave_split

CFLAGS = `pkg-config glib-2.0 --cflags` -I ../wavelib/include -g -Wall -Werror -pedantic

LIBS = `pkg-config glib-2.0 --libs`

wave_split: wave_split.o wave.o
	gcc wave_split.o wave.o $(LIBS) -o wave_split

wave_split.o: wave_split.c
	gcc $(CFLAGS) -c wave_split.c

wave.o: ../wavelib/src/wave.c
	gcc $(CFLAGS) -c ../wavelib/src/wave.c

clean:
	rm -rf *.o *.i
