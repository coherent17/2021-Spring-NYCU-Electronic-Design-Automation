CC = g++
CFLAGS = -g -Wall -O3
CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --track-origins=yes
OBJS = parsor.o GainList.o FiducciaMattheysesAlgorithm.o
BINS = main

all: $(BINS)


%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BINS): $(BINS).cpp $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

sample:
	./$(BINS) sample.hgr

do1:
	./$(BINS) input.hgr

do2:
	./$(BINS) input2.txt.hgr

do3:
	./$(BINS) input3.txt.hgr

checksample:
	$(CHECKCC) $(CHECKFLAGS) ./$(BINS) sample.hgr

check1:
	$(CHECKCC) $(CHECKFLAGS) ./$(BINS) input.hgr

check2:
	$(CHECKCC) $(CHECKFLAGS) ./$(BINS) input2.txt.hgr

check3:
	$(CHECKCC) $(CHECKFLAGS) ./$(BINS) input3.txt.hgr

clean:
	rm $(BINS) $(OBJS)