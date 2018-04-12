CC = gcc
CFLAGS = -Wall

IFLAGS = -I./ -I/usr/local/include/google/
LFLAGS = -L./ -L/usr/local/lib/ -lkmeansd -lcmockery

DFLAGS = -D UNIT_TESTING=1

OBJ = kmeans
TEST = kmeans_test
LIB = libkmeans.a
LIBD = libkmeansd.a

all: $(OBJ).o
	ar crs $(LIB) $<

clean:
	rm -f *.o $(TEST) $(LIB) $(LIBD)

test: $(TEST)
	./$(TEST)

$(OBJ).o: $(OBJ).c
	$(CC) $(CFLAGS) -o $@ -c $<

$(TEST): $(TEST).c $(LIBD)
	$(CC) $(CFLAGS) -o $@ $< $(IFLAGS) $(LFLAGS) $(DFLAGS)

$(LIBD): $(OBJ)d.o
	ar crs $@ $<

$(OBJ)d.o: $(OBJ).c
	$(CC) $(CFLAGS) -o $@ -c $< $(DFLAGS)
