
ROOT=../..
CC=gcc
CFLAGS= -g -I$(ROOT)/src

SRC=main.c $(ROOT)/src/tinflate.c $(ROOT)/src/adler32.c $(ROOT)/src/crc32.c $(ROOT)/src/tinfgzip.c
BIN=tgunzip

all: $(BIN)

$(BIN): $(SRC) $(INCS)
	$(CC) $(CFLAGS) $(SRC) -o $@

clean:
	rm -rf *.o $(BIN) $(BIN).dSYM/
