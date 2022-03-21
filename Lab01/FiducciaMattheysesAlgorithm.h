#include <vector>
#include <unordered_set>

using namespace std;
using std::vector;
using std::unordered_set;

typedef struct vector<unordered_set<int>> vu;

#ifndef _FIDUCCIA_MATTHEYSES_ALGORITHM_H_
#define _FIDUCCIA_MATTHEYSES_ALGORITHM_H_

int getMaxGainCell(vu &leftGainList, vu &rightGainList, bool *comeFrom, int leftPartitionCellCount, int rightPartitionCellCount);
void removeFromBucketList(int move_cell_id, bool comeFrom, vu &leftGainList, vu &rightGainList, vector <int> &gain);
void updateLockState(int move_cell_id, vector <int> &CellLockState);
void updatePartition(int move_cell_id, vector <int> &partition, int *leftPartitionCellCount, int *rightPartitionCellCount, bool comeFrom);
void updateNeighborGain(vu &leftGainList, vu &rightGainList, vu &netArray, vu &cellArray, vector <int> &gain, int move_cell_id, vector <int> &partition, vector <int> &CellLockState);
void outputBestPartition(vector <int> &best_partition);
#endif