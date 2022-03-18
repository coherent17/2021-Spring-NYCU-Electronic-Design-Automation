#include "FiducciaMattheysesAlgorithm.h"
#include <stdbool.h>
#include <vector>
#include <unordered_set>

using namespace std;
using std::vector;
using std::unordered_set;

vector <bool> init_partition(int nodeNumber, int *leftPartitionCellCount, int *rightPartitionCellCount){
	vector <bool> partition ((nodeNumber + 1));
	*leftPartitionCellCount = 0;
	*rightPartitionCellCount = 0;
	for(int i = 1; i <=nodeNumber; i++){
		if((i % 2 + 1) % 2 == 0) (*leftPartitionCellCount)++ ;
		else (*rightPartitionCellCount)++;
		partition[i] = (i % 2 + 1) % 2;
	}
	return partition;
}

void printPartition(vector <bool> &partition, int nodeNumber, int leftPartitionCellCount, int rightPartitionCellCount){
	for(int i = 1; i <= nodeNumber; i++){
		if(partition[i] == 0){
			printf("%d : left\n", i);
		}
		else printf("%d : right\n", i);
	}
	printf("\n");
	printf("leftPartitionCellCount = %d, rightPartitionCellCount = %d\n", leftPartitionCellCount, rightPartitionCellCount);
}

//outer loop
int FS(vector<unordered_set<int>> &netArray, vector<unordered_set<int>> &cellArray, vector <bool> &partition, int targetCell, int netNumber, int nodeNumber){
	int result = 0;
	int nums_net_cell_on_oneside = 0;
	for(const auto &i : cellArray[targetCell]){
		if(netArray[i].count(targetCell)){
			//why???????????????????????
			nums_net_cell_on_oneside = 1;
			for(const auto &s : netArray[i]){
				if(netArray[i].count(s) && s!= targetCell){
					if(partition[s] == partition[targetCell]){
						nums_net_cell_on_oneside = 0;
						break;
					}
				}
			}
			result += nums_net_cell_on_oneside;
		}
	}
	return result;
}

int TE(vector<unordered_set<int>> &netArray, vector<unordered_set<int>> &cellArray, vector <bool> &partition, int targetCell, int netNumber, int nodeNumber){
	int result = 0;
	int nums_net_cell_on_oneside = 0;
	for(const auto &i : cellArray[targetCell]){
		if(netArray[i].count(targetCell)){
			//why???????????????????????
			nums_net_cell_on_oneside = 1;
			for(const auto &s : netArray[i]){
				if(netArray[i].count(s) && s!= targetCell){
					if(partition[s] != partition[targetCell]){
						nums_net_cell_on_oneside = 0;
						break;
					}
				}
			}
			result += nums_net_cell_on_oneside;
		}
	}
	return result;	
}

vector <int> calculate_all_gain(vector<unordered_set<int>> &netArray, vector<unordered_set<int>> &cellArray, vector <bool> &partition, int netNumber, int nodeNumber){
	vector <int> gain (nodeNumber + 1);
	for(int i = 1; i <= nodeNumber; i++){
		gain[i] = FS(netArray, cellArray, partition, i, netNumber, nodeNumber) - TE(netArray, cellArray, partition, i, netNumber, nodeNumber);
	}
	return gain;
}

void printGain(vector <int> &gain, int nodeNumber){
	for(int i = 1; i <= nodeNumber; i++){
		printf("%d ", gain[i]);
	}
	printf("\n");
}

void BuildGainList(vector <int> &gain, int max_terminal, vector<unordered_set<int>> &leftGainList, vector<unordered_set<int>> &rightGainList, vector <bool> &partition){
	for(int i = 1; i < (int)gain.size(); i++){
		//printf("%d insert at gain = %d list, index = %d\n", i, gain[i], gain[i] + max_terminal);
		if(partition[i] == false) leftGainList[gain[i] + max_terminal].insert(i);
		else if(partition[i] == true) rightGainList[gain[i] + max_terminal].insert(i);
	}
}

void printGainList(vector <unordered_set<int>> GainList, int max_terminal){
	printf("max_terminal = %d \n", max_terminal);
	int i = 2 * max_terminal;
	while(i >= 0){
		printf("Gain = %d :", i - max_terminal);
		for(const auto &t : GainList[i]){
			printf("%d ", t);
		}
		printf("\n");
		i--;
	}
	printf("\n");
}

vector <bool> BuildCellLockState(int nodeNumber){
	//define (0, 1) = (unlock, lock)
	vector <bool> CellLockState (nodeNumber + 1, 0);
	return CellLockState;
}

void printCellLockState(vector <bool> &CellLockState){
	for(int i = 1; i < (int)CellLockState.size(); i++){
		if(CellLockState[i] == 0) printf("%d cell: Unlocked\n", i);
		else printf("%d cell: Locked\n", i);
	}
}