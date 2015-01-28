CC=gcc
CFLAGS=-Wall -I. -DARM_MARKERS -DGEM5_MARKERS
LDFLAGS=
DEPS = marker.h
OBJ = test.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

marker_test: $(OBJ)
	gcc -o $@ $^ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.o
	rm -f marker_test
