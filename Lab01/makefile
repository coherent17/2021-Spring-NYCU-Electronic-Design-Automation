CC = g++
CFLAGS = -g -Wall -O3
OBJS = parsor.o GainList.o FiducciaMattheysesAlgorithm.o
BINS = Lab1
OUT = output.txt

all: $(BINS)

%.o: %.cpp %.h
	$(CC) $(CFLAGS) -c $< -o $@

$(BINS): main.cpp $(OBJS)
	$(CC) $(CFLAGS) -fopenmp $^ -o $@

do1:
	./$(BINS) input.hgr

do2:
	./$(BINS) input2.txt.hgr

do3:
	./$(BINS) input3.txt.hgr

do_shmetis:
	chmod +x shmetis
	./shmetis input3.txt.hgr 2 5

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