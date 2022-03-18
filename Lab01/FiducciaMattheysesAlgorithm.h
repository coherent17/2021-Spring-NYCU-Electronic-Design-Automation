#include <vector>
#include <unordered_set>

using namespace std;
using std::vector;
using std::unordered_set;

typedef struct vector<unordered_set<int>> vu;

#ifndef _FIDUCCIA_MATTHEYSES_ALGORITHM_H_
#define _FIDUCCIA_MATTHEYSES_ALGORITHM_H_

int getMaxGainCell(vector<unordered_set<int>> &leftGainList, vector<unordered_set<int>> &rightGainList, bool *comeFrom, int max_terminal, int leftPartitionCellCount, int rightPartitionCellCount);
void removeFromBucketList(int move_cell_id, bool comeFrom, vu &leftGainList, vu &rightGainList, vector <int> &gain, int max_terminal);
void updateLockState(int move_cell_id, vector <bool> &CellLockState);
void updatePartition(int move_cell_id, vector <bool> &partition, int *leftPartitionCellCount, int *rightPartitionCellCount, bool comeFrom);
void updateNeighborGain(vu &leftGainList, vu &rightGainList, vu &netArray, vu &cellArray, vector <int> &gain, int move_cell_id, int netNumber, int nodeNumber, vector <bool> &partition, vector <bool> &CellLockState, int max_terminal);

#endif