CC = gcc
CFLAGS = -g -Wall
CUDD = ../cudd-3.0.0
INCLUDE = -I $(CUDD)/cudd -I $(CUDD)/util -I $(CUDD)
LINKER = $(CUDD)/cudd/.libs/libcudd.a -lm
BINS = and or boolean ab+cd
OUTPUT = and_out or_out boolean_out ab+cd_out

CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --show-leak-kinds=all


all: $(BINS)

%: %.c
	$(CC) $(CFLAGS) $(INCLUDE) $^ -o $@ $(LINKER) 

clean:
	rm -rf $(BINS) $(OUTPUT)