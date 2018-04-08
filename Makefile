CC = gcc
CFLAGS = -Wall

IFLAGS = -I./ -I/usr/local/include/google/
LFLAGS = -L./ -L/usr/local/lib/ -lkmeans -lcmockery

OBJ = kmeans
TEST = tests
LIB = libkmeans.a

all: $(LIB)

clean:
	rm -f *.o $(TEST) $(LIB)

test: $(LIB) $(TEST)
	./$(TEST)

$(LIB): $(OBJ).o
	ar crs $@ $<

$(OBJ).o: $(OBJ).c
	$(CC) $(CFLAGS) -o $@ -c $<

$(TEST): $(TEST).c
	$(CC) $(CFLAGS) -o $@ $< $(IFLAGS) $(LFLAGS)
