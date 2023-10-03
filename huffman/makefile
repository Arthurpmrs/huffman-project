CC=gcc
CFLAGS=-Wall

DEPS = libs/huffman.h libs/linked_list.h libs/binary_tree.h libs/utils.h
TEST_DEPS = tests/test_binary_tree.h tests/test_huffman.h tests/test_linked_list.h

ZIP_OBJ = zip.o libs/huffman.o libs/linked_list.o libs/utils.o
UNZIP_OBJ = unzip.o libs/binary_tree.o libs/utils.o
TEST_OBJ = tests/tests.o libs/binary_tree.o tests/test_binary_tree.o libs/huffman.o tests/test_huffman.o tests/test_linked_list.o libs/linked_list.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

zip: $(ZIP_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

unzip: $(UNZIP_OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: %.c $(TEST_DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

tests: $(TEST_OBJ)
	$(CC) -o tests/$@ $^ $(CFLAGS) -lcunit

clean:
	-rm -f $(ZIP_OBJ)
	-rm -f $(UNZIP_OBJ)
	-rm -f $(TEST_OBJ)
	-rm -f zip
	-rm -f unzip
	-rm -f tests/tests