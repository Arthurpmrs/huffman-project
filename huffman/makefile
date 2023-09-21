CC=gcc
CFLAGS=-Wall

DEPS = libs/binary_tree.h
TEST_DEPS = tests/test_binary_tree.h

UNZIP_OBJ = unzip.o libs/binary_tree.o
TEST_OBJ = tests/tests.o libs/binary_tree.o tests/test_binary_tree.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

unzip: $(UNZIP_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(TEST_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tests: $(TEST_OBJ)
	$(CC) -o tests/$@ $^ $(CFLAGS) -lcunit

clean:
	-rm -f $(UNZIP_OBJ)
	-rm -f $(TEST_OBJ)
	-rm -f unzip
	-rm -f tests/tests