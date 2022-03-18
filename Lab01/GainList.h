#include <vector>
#include <unordered_set>

using namespace std;
using std::vector;
using std::unordered_set;

#ifndef _GAINLIST_H_
#define _GAINLIST_H_

vector <bool> 	init_partition(int nodeNumber, int *leftPartitionCellCount, int *rightPartitionCellCount);
void 			printPartition(vector <bool> &partition, int nodeNumber, int leftPartitionCellCount, int rightPartitionCellCount);
int 			FS(vector<unordered_set<int>> &netArray, vector<unordered_set<int>> &cellArray, vector <bool> &partition, int targetCell, int netNumber, int nodeNumber);
int 			TE(vector<unordered_set<int>> &netArray, vector<unordered_set<int>> &cellArray, vector <bool> &partition, int targetCell, int netNumber, int nodeNumber);
vector <int> 	calculate_all_gain(vector<unordered_set<int>> &netArray, vector<unordered_set<int>> &cellArray, vector <bool> &partition, int netNumber, int nodeNumber);
void 			printGain(vector <int> &gain, int nodeNumber);
void 			BuildGainList(vector <int> &gain, int max_terminal, vector<unordered_set<int>> &leftGainList, vector<unordered_set<int>> &rightGainList, vector <bool> &partition);
void 			printGainList(vector <unordered_set<int>> GainList, int max_terminal);
vector <bool> 	BuildCellLockState(int nodeNumber);
void 			printCellLockState(vector <bool> &CellLockState);

#endif