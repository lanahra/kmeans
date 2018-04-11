CC = gcc
CFLAGS = -Wall

IFLAGS = -I./ -I/usr/local/include/google/
LFLAGS = -L./ -L/usr/local/lib/ -lkmeans -lcmockery

OBJ = kmeans
TEST = kmeans_test
LIB = libkmeans.a

all: $(LIB)

clean:
	rm -f *.o $(TEST) $(LIB)

test: $(TEST)
	./$(TEST)

$(LIB): $(OBJ).o
	ar crs $@ $<

$(OBJ).o: $(OBJ).c
	$(CC) $(CFLAGS) -o $@ -c $<

$(TEST): $(TEST).c $(LIB)
	$(CC) $(CFLAGS) -o $@ $< $(IFLAGS) $(LFLAGS) -D UNIT_TESTING=1
