CC = gcc
CFLAGS = -Wall -Wno-pointer-to-int-cast

IFLAGS = -I./ -I/usr/local/include/google/
LFLAGS = -L./ -L/usr/local/lib/ -lcmockery

DFLAGS = -D UNIT_TESTING=1

OBJ = kmeans
TEST = kmeans_test
LIB = libkmeans.a
LIBD = libkmeansd.a
MAIN = main

all: $(OBJ).o
	ar crs $(LIB) $<
	$(CC) $(CFLAGS) -o $(MAIN) $(MAIN).c $(IFLAGS) $(LFLAGS) -lkmeans

clean:
	rm -f *.o $(TEST) $(LIB) $(LIBD) $(MAIN)

test: $(TEST)
	./$(TEST)

$(OBJ).o: $(OBJ).c
	$(CC) $(CFLAGS) -o $@ -c $<

$(TEST): $(TEST).c $(LIBD)
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ $< $(IFLAGS) $(LFLAGS) -lkmeansd

$(LIBD): $(OBJ)d.o
	ar crs $@ $<

$(OBJ)d.o: $(OBJ).c
	$(CC) $(CFLAGS) $(DFLAGS) -o $@ -c $< -lkmeansd
