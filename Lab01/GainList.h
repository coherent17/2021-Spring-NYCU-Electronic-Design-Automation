#include <vector>
#include <unordered_set>

using namespace std;
using std::vector;
using std::unordered_set;

typedef vector<unordered_set<int>> vu;

#ifndef _GAINLIST_H_
#define _GAINLIST_H_

vector <int> 	init_partition(int *leftPartitionCellCount, int *rightPartitionCellCount);
void 			printPartition(vector <int> &partition, int leftPartitionCellCount, int rightPartitionCellCount);
int 			FS(vu &netArray, vu &cellArray, vector <int> &partition, int targetCell);
int 			TE(vu &netArray, vu &cellArray, vector <int> &partition, int targetCell);
vector <int> 	calculate_all_gain(vu &netArray, vu &cellArray, vector <int> &partition);
void 			printGain(vector <int> &gain);
void 			BuildGainList(vector <int> &gain, vu &leftGainList, vu &rightGainList, vector <int> &partition);
void 			printGainList(vu GainList);
vector <int> 	BuildCellLockState();
void 			unlockClockState(vector <int>& CellLockState);
void 			printCellLockState(vector <int> &CellLockState);

#endif