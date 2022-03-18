#include <vector>
#include <unordered_set>

using namespace std;
using std::vector;
using std::unordered_set;

#ifndef _FIDUCCIA_MATTHEYSES_ALGORITHM_H_
#define _FIDUCCIA_MATTHEYSES_ALGORITHM_H_

int getMaxGainCell(vector<unordered_set<int>> &leftGainList, vector<unordered_set<int>> &rightGainList, bool *comeFrom, int max_terminal, int leftPartitionCellCount, int rightPartitionCellCount);

#endif