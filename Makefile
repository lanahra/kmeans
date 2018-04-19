CC = gcc
CFLAGS = -Wall -Wno-pointer-to-int-cast

IFLAGS = -I/usr/local/include/google/
LFLAGS = -L/usr/local/lib/ -lcmockery -lpthread

DFLAGS = -D UNIT_TESTING=1

OBJ = main.o kmeans.o
TST = kmeans_test.o kmeansd.o

all: $(OBJ)
	$(CC) $(CFLAGS) -o kmeans $(OBJ) -lpthread

clean:
	rm -f *.o kmeans kmeans_test

test: $(TST)
	$(CC) $(CFLAGS) $(DFLAGS) -o kmeans_test $(TST) $(IFLAGS) $(LFLAGS)
	./kmeans_test

kmeans_test.o: kmeans_test.c
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ -c $< $(IFLAGS) $(LFLAGS)

kmeansd.o: kmeans.c
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ -c $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<
