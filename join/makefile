all: wave_join

CFLAGS = `pkg-config glib-2.0 --cflags` -I ../wavelib/include -g -Wall -Werror -pedantic

LIBS = `pkg-config glib-2.0 --libs`

wave_join: wave_join.o wave.o
	gcc wave_join.o wave.o $(LIBS) -o wave_join

wave_join.o: wave_join.c
	gcc $(CFLAGS) -c wave_join.c

wave.o: ../wavelib/src/wave.c
	gcc $(CFLAGS) -c ../wavelib/src/wave.c

clean:
	rm -rf *.o *.i
