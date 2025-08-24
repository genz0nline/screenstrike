CC=gcc

SRC=screenstrike.c

CFLAGS=$(shell pkg-config --cflags libdrm) -ldrm

target: screenstrike

screenstrile: $(SRC)
	$(CC) -o screenstrike $(SRC) $(CFLAGS)

clean:
	rm -rf *.o *.d screenstrike
