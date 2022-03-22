#include <stdbool.h>
#include <vector>
#include <unordered_set>

using namespace std;
using std::vector;
using std::unordered_set;

extern int netNumber;
extern int nodeNumber;
extern int max_terminal;

typedef vector<unordered_set<int>> vu;

vector <int> init_partition(int *leftPartitionCellCount, int *rightPartitionCellCount){
	vector <int> partition ((nodeNumber + 1));
	*leftPartitionCellCount = 0;
	*rightPartitionCellCount = 0;
	for(int i = 1; i <=nodeNumber; i++){
		partition[i] = 0;
		(*leftPartitionCellCount)++;
	}
	return partition;
}

void printPartition(vector <int> &partition, int leftPartitionCellCount, int rightPartitionCellCount){
	/*for(int i = 1; i <= nodeNumber; i++){
		if(partition[i] == 0){
			printf("%d : left\n", i);
		}
		else printf("%d : right\n", i);
	}
	printf("\n");*/
	printf("leftPartitionCellCount = %d, rightPartitionCellCount = %d\n", leftPartitionCellCount, rightPartitionCellCount);
}

int FS(vu &netArray, vu &cellArray, vector <int> &partition, int targetCell){
	int result = 0;
	int nums_net_cell_on_oneside = 0;
	for(const auto &i : cellArray[targetCell]){
		if(netArray[i].count(targetCell)){
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

int TE(vu &netArray, vu &cellArray, vector <int> &partition, int targetCell){
	int result = 0;
	int nums_net_cell_on_oneside = 0;
	for(const auto &i : cellArray[targetCell]){
		if(netArray[i].count(targetCell)){
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

vector <int> calculate_all_gain(vu &netArray, vu &cellArray, vector <int> &partition){
	vector <int> gain (nodeNumber + 1);

	for(int i = 1; i <= nodeNumber; i++){
		gain[i] = FS(netArray, cellArray, partition, i) - TE(netArray, cellArray, partition, i);
	}
	return gain;
}

void printGain(vector <int> &gain){
	for(int i = 1; i <= nodeNumber; i++){
		printf("%d ", gain[i]);
	}
	printf("\n");
}

void BuildGainList(vector <int> &gain, vu &leftGainList, vu &rightGainList, vector <int> &partition){
	for(int i = 1; i < (int)gain.size(); i++){
		if(partition[i] == 0) leftGainList[gain[i] + max_terminal].insert(i);
		else if(partition[i] == 1) rightGainList[gain[i] + max_terminal].insert(i);
	}
}

void printGainList(vu GainList){
	int i = 2 * max_terminal;
	while(i >= 0){
		printf("Gain = %d\t:", i - max_terminal);
		for(const auto &t : GainList[i]){
			printf("%d ", t);
		}
		printf("\n");
		i--;
	}
	printf("\n");
}

vector <int> BuildCellLockState(){
	//define (0, 1) = (unlock, lock)
	vector <int> CellLockState (nodeNumber + 1, 0);
	return CellLockState;
}

void unlockClockState(vector <int>& CellLockState){
	for(int i = 1; i <= nodeNumber; i++){
		CellLockState[i] = 0;
	}
}

void printCellLockState(vector <int> &CellLockState){
	for(int i = 1; i < (int)CellLockState.size(); i++){
		if(CellLockState[i] == 0) printf("%d cell: Unlocked\n", i);
		else printf("%d cell: Locked\n", i);
	}
}