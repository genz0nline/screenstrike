CC=gcc

SRC=screenstrike.c

target: screenstrike

screenstrile: $(SRC)
	$(CC) -o screenstrike $(SRC)

clean:
	rm -rf *.o *.d screenstrike
