#include "parsor.h"
#include "GainList.h"
#include "FiducciaMattheysesAlgorithm.h"
#include <vector>
#include <unordered_set>

using namespace std;
using std::vector;
using std::unordered_set;

typedef struct vector<unordered_set<int>> vu;

int main(int argc, char *argv[]){
	char *input_filename = *(argv + 1);

	int netNumber;													//the net count of the netlist
	int nodeNumber;													//the cell count of the netlist
	int max_terminal;       										//the maximum net that the cell connect to

	vector<unordered_set<int>> netArray;							//the 2D array to store the cell which the net connect to
	vector<unordered_set<int>> cellArray;							//the 2D array to store the net that the cell connect to

	vector <bool> partition; 										//the partition array to store whether the cell in which partition

	int leftPartitionCellCount;										//the number of the cell in left partition
	int rightPartitionCellCount;									//the number of the cell in right partition

	vector <int> gain;												//store the initial partition gain of each cell

	vector <bool> CellLockState;									//the array to store whether the cell is locked or not

	netArray = BuildNetArray(input_filename, &netNumber, &nodeNumber);
	cellArray = BuildCellArray(input_filename, &max_terminal);

	vector<unordered_set<int>> LeftGainList(2 * max_terminal + 1);	//the left bucket gainlist
	vector<unordered_set<int>> RightGainList(2 * max_terminal + 1); //the right bucket gainlist
	//printNetArray(netArray);
	//printCellArray(cellArray);

	partition =  init_partition(nodeNumber, &leftPartitionCellCount, &rightPartitionCellCount);
	//printPartition(partition, nodeNumber, leftPartitionCellCount, rightPartitionCellCount);
	gain = calculate_all_gain(netArray, cellArray, partition, netNumber, nodeNumber);
	//printGain(gain, nodeNumber);

	BuildGainList(gain, max_terminal, LeftGainList, RightGainList, partition);
	//printGainList(LeftGainList, max_terminal);
	//printGainList(RightGainList, max_terminal);

	CellLockState = BuildCellLockState(nodeNumber);


	printf("------------------------testing---------------------\n");
	int move_cell_id;
	bool comeFrom;

	int totalGain = 0;
	int currentMaxGain = 0;
	vector <bool> best_partition;
	best_partition.assign(partition.begin(), partition.end());

	for(int i = 1; i <= nodeNumber; i++){
		move_cell_id = getMaxGainCell(LeftGainList, RightGainList, &comeFrom, max_terminal, leftPartitionCellCount, rightPartitionCellCount);
		totalGain += gain[move_cell_id];
		//printf("choose cell %d to move\n", move_cell_id);
		//printf("remove %d from bucketlist\n", move_cell_id);
		removeFromBucketList(move_cell_id, comeFrom, LeftGainList, RightGainList, gain, max_terminal);
		updateLockState(move_cell_id, CellLockState);
		updatePartition(move_cell_id, partition, &leftPartitionCellCount, &rightPartitionCellCount, comeFrom);
		//printPartition(partition, nodeNumber, leftPartitionCellCount, rightPartitionCellCount);
		if(totalGain >= currentMaxGain){
			currentMaxGain = totalGain;
			best_partition.assign(partition.begin(), partition.end());
		}
		updateNeighborGain(LeftGainList, RightGainList, netArray, cellArray, gain, move_cell_id, netNumber, nodeNumber, partition, CellLockState, max_terminal);
		//printf("---------------------------update gain list----------------\n");
		//printGainList(LeftGainList, max_terminal);
		//printGainList(RightGainList, max_terminal);
		//printGain(gain, nodeNumber);
		//printf("best_partition\n");
		//printPartition(best_partition, nodeNumber, leftPartitionCellCount, rightPartitionCellCount);
	}
	//printPartition(best_partition, nodeNumber, leftPartitionCellCount, rightPartitionCellCount);
	//printf("max gain = %d\n", currentMaxGain);

	FILE *output = fopen("output.txt", "w");
	for(int i = 1; i <= nodeNumber; i++){
		fprintf(output, "%d\n", (int)best_partition[i]);
	}
	fclose(output);
	return 0;
}