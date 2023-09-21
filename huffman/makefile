CC=gcc
CFLAGS=-Wall

DEPS = libs/binary_tree.h

UNZIP_OBJ = unzip.o libs/binary_tree.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

unzip: $(UNZIP_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

clean:
	-rm -f $(UNZIP_OBJ)
	-rm -f unzip