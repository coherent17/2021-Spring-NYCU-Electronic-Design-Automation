#include <vector>
#include <unordered_set>

using namespace std;
using std::vector;
using std::unordered_set;

typedef struct vector<unordered_set<int>> vu;

#ifndef _PARSOR_H_
#define _PARSOR_H_
	vu BuildNetArray(char *filename, int *netNumber, int *nodeNumber);
	vu BuildCellArray(char *filename, int *max_terminal);
	void printNetArray(vu netArray);
	void printCellArray(vu cellArray);
#endif