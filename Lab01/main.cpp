#include "parsor.h"
#include "GainList.h"
#include "FiducciaMattheysesAlgorithm.h"
#include <vector>
#include <unordered_set>

using namespace std;
using std::vector;
using std::unordered_set;

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

	bool comeFrom;

	netArray = BuildNetArray(input_filename, &netNumber, &nodeNumber);
	cellArray = BuildCellArray(input_filename, &max_terminal);

	vector<unordered_set<int>> LeftGainList(2 * max_terminal + 1);	//the left bucket gainlist
	vector<unordered_set<int>> RightGainList(2 * max_terminal + 1); //the right bucket gainlist
	//printNetArray(netArray);
	//printCellArray(cellArray);

	partition =  init_partition(nodeNumber, &leftPartitionCellCount, &rightPartitionCellCount);
	printPartition(partition, nodeNumber, leftPartitionCellCount, rightPartitionCellCount);
	gain = calculate_all_gain(netArray, cellArray, partition, netNumber, nodeNumber);
	printGain(gain, nodeNumber);

	BuildGainList(gain, max_terminal, LeftGainList, RightGainList, partition);
	printGainList(LeftGainList, max_terminal);
	printGainList(RightGainList, max_terminal);

	CellLockState = BuildCellLockState(nodeNumber);
	//printCellLockState(CellLockState);

	int cell = getMaxGainCell(LeftGainList, RightGainList, &comeFrom, max_terminal, leftPartitionCellCount, rightPartitionCellCount);
	printf("cell %d, come from %d\n", cell, comeFrom);
	return 0;
}