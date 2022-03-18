#include <vector>
#include <unordered_set>

using namespace std;
using std::vector;
using std::unordered_set;

#ifndef _PARSOR_H_
#define _PARSOR_H_
	vector<unordered_set<int>> BuildNetArray(char *filename, int *netNumber, int *nodeNumber);
	vector<unordered_set<int>> BuildCellArray(char *filename, int *max_terminal);
	void printNetArray(vector<unordered_set<int>> netArray);
	void printCellArray(vector<unordered_set<int>> cellArray);
#endif