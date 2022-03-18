CC = g++
CFLAGS = -g -Wall -O3
CHECKCC = valgrind
CHECKFLAGS = --leak-check=full -s --track-origins=yes
OBJS = parsor.o GainList.o FiducciaMattheysesAlgorithm.o
BINS = Lab1
OUT = output.txt

all: $(BINS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BINS): main.cpp $(OBJS)
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

verifier1:
	chmod +x verifier
	./verifier input.hgr output.txt

verifier2:
	chmod +x verifier
	./verifier input2.txt.hgr output.txt

verifier3:
	chmod +x verifier
	./verifier input3.txt.hgr output.txt

runall:
	chmod +x verifier
	time ./$(BINS) input.hgr
	./verifier input.hgr $(OUT)
	time ./$(BINS) input2.txt.hgr
	./verifier input2.txt.hgr $(OUT)
	time ./$(BINS) input3.txt.hgr $(OUT)
	./verifier input3.txt.hgr $(OUT)

clean:
	rm $(BINS) $(OBJS) $(OUT)