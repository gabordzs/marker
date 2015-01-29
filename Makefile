CC=gcc
CFLAGS=-Wall -I.
LDFLAGS=
DEPS=marker_stub.h marker.h
OBJ=test.o
SRC=test.c
TESTS=marker_no_test marker_gem5_test marker_arm_test

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

all: $(TESTS)

marker_no_test: $(DEPS) $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

marker_arm_test: $(DEPS) $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -DARM_MARKERS

marker_gem5_test: $(DEPS) $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS) -DARM_MARKERS -DGEM5_MARKERS

install:
	sudo cp marker.h /usr/include

clean:
	rm -f *.o
	rm -f $(TESTS)
