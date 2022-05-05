CC = g++
CFLAGS = -Wall
CUDD = ../cudd-3.0.0
INCLUDE = -I $(CUDD)/cudd -I $(CUDD)/util -I $(CUDD)
LINKER = $(CUDD)/cudd/.libs/libcudd.a -lm
BINS = Lab3
OBJS = readfile.o buildBDD.o

CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --show-leak-kinds=all --track-origins=yes



all: $(BINS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) $(INCLUDE) -c  $< -o $@

$(BINS): main.cpp $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@ $(LINKER)

runall:
	./$(BINS) ../case1.txt mycase1.out
	./$(BINS) ../case2.txt mycase2.out
	./$(BINS) ../case3.txt mycase3.out

seeall:
	cat mycase1.out
	cat mycase2.out
	cat mycase3.out

clean:
	rm -rf *.o $(BINS) *.out *.dot *.png