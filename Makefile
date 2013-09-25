CFLAGS=-ansi -Wall

test: test_sync test_async

test_sync: test_sync.c librancio.a
	$(CC) -o $@ $(CFLAGS) -L. $@.c -lrancio

# Not ansi
test_async: test_async.c librancio.a
	$(CC) -o $@ $(filter-out -ansi,$(CFLAGS)) -L. $@.c -lrancio

librancio.a: rancio.o
	$(AR) cr $@ $<

rancio.o: rancio.c rancio.h

clean:
	rm -fr test_sync test_async *.o *.a

.PHONY: clean test
