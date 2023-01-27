all: wave_record

CFLAGS = `pkg-config libwave --cflags` -g -Wall -Werror -pedantic

LIBS = `pkg-config libwave --libs` -lasound -pthread

wave_record: wave_record.o
	gcc wave_record.o $(LIBS) -o wave_record

wave_record.o: wave_record.c
	gcc $(CFLAGS) -c wave_record.c

clean:
	rm -rf *.o *.i
