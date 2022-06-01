CC = g++
CFLAGS = -Wall -O3
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

do1:
	./$(BINS) ../case1.txt mycase1.out
	dot -Tsvg 1_order.dot -o 1_order.svg
	dot -Tsvg 2_order.dot -o 2_order.svg

do2:
	./$(BINS) ../case2.txt mycase2.out
	dot -Tsvg 1_order.dot -o 1_order.svg
	dot -Tsvg 2_order.dot -o 2_order.svg

do3:
	./$(BINS) ../case3.txt mycase3.out
	dot -Tsvg 1_order.dot -o 1_order.svg
	dot -Tsvg 2_order.dot -o 2_order.svg
	dot -Tsvg 3_order.dot -o 3_order.svg

do4:
	./$(BINS) ../case4.txt mycase4.out

do5:
	./$(BINS) ../case5.txt mycase5.out

do6:
	./$(BINS) ../case6.txt mycase6.out

runall:
	time ./$(BINS) ../case1.txt mycase1.out
	time ./$(BINS) ../case2.txt mycase2.out
	time ./$(BINS) ../case3.txt mycase3.out
	time ./$(BINS) ../case4.txt mycase4.out
	time ./$(BINS) ../case5.txt mycase5.out
	time ./$(BINS) ../case6.txt mycase6.out

seeall:
	cat mycase1.out
	cat mycase2.out
	cat mycase3.out
	cat mycase4.out
	cat mycase5.out
	cat mycase6.out

clean:
	rm -rf *.o $(BINS) *.out *.dot *.svg