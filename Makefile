CC = clang++
CFLAGS = -Wall -Wextra -std=c++11 -c
LFLAGS = -Wall -Wextra 
HEAD = grid.h
OS = grid_test.o grid.o

%.o: %.cpp $(HEAD)
	$(CC) -o $@ $< $(CFLAGS)

all: grid_test.o grid.o
	$(CC) -o test_grid grid_test.o grid.o $(LFLAGS)

grid: grid_test.o grid.o
	$(CC) -o test_grid grid_test.o grid.o $(LFLAGS)

clean:
	rm -f *.o core* *~