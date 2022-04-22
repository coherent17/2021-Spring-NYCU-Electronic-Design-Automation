CC = g++
CFLAGS = -g -Wall -O3
BINS = 0811562_lab2.o
SRC_DIRS := src/lib

CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --show-leak-kinds=all

SRCS := $(wildcard $(SRC_DIRS:=/*.cpp))
OBJS := $(SRCS:.cpp=.o)
OUT = *.svg *.out

all: $(BINS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c  $< -o $@

$(BINS): src/main.cpp $(OBJS)
	$(CC) $(CFLAGS)  $^ -o $@

do:
	$(CHECKCC) $(CHECKFLAGS) ./$(BINS) case3.in output.out

verifier:
	chmod +x draw

	./$(BINS) case1.in case1.out
	./draw case1.in case1.out

	./$(BINS) case2.in case2.out
	./draw case2.in case2.out

	./$(BINS) case3.in case3.out
	./draw case3.in case3.out

	./$(BINS) sample.in sample.out
	./draw sample.in sample.out

clean:
	rm -rf src/lib/*.o $(BINS) $(OUT)