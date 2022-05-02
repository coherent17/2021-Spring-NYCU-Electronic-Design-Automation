CC = gcc
CFLAGS = -g -Wall
CUDD = cudd-3.0.0
INCLUDE = -I $(CUDD)/cudd -I $(CUDD)/util -I $(CUDD)
LINKER = $(CUDD)/cudd/.libs/libcudd.a -lm
BINS = Lab3
SRC_DIRS := src/lib


CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --show-leak-kinds=all

SRCS := $(wildcard $(SRC_DIRS:=/*.cpp))
OBJS := $(SRCS:.cpp=.o)

all: $(BINS)
%.o: %.c %.h
	$(CC) $(CFLAGS) $(INCLUDE) -c  $< -o $@ $(LINKER) 

$(BINS): src/main.c $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@ $(LINKER) 

clean:
	rm -rf src/lib/*.o $(BINS)