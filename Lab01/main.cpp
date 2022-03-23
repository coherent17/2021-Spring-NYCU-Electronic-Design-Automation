#include "parsor.h"
#include "GainList.h"
#include "FiducciaMattheysesAlgorithm.h"
#include <vector>
#include <unordered_set>
#include <omp.h>

#define CUT_OFF_TIME 28

using namespace std;
using std::vector;
using std::unordered_set;

typedef vector<unordered_set<int>> vu;

//extern global variable
int netNumber;															//the net count of the netlist
int nodeNumber;															//the cell count of the netlist
int max_terminal;														//the maximum net that the cell connect to

int main(int argc, char *argv[]){
	char *input_filename = *(argv + 1);

	double start_time = omp_get_wtime();								//get the time that the program start


	vector<unordered_set<int>> netArray;								//the 2D array to store the cell which the net connect to
	vector<unordered_set<int>> cellArray;								//the 2D array to store the net that the cell connect to

	vector <int> partition; 											//the partition array to store whether the cell in which partition

	int leftPartitionCellCount;											//the number of the cell in left partition
	int rightPartitionCellCount;										//the number of the cell in right partition

	vector <int> gain;													//store the initial partition gain of each cell
	vector <int> CellLockState;											//the array to store whether the cell is locked or not

	netArray = BuildNetArray(input_filename, &netNumber, &nodeNumber);
	cellArray = BuildCellArray(input_filename, &max_terminal);

	vu LeftGainList(2 * max_terminal + 1);								//the left bucket gainlist
	vu RightGainList(2 * max_terminal + 1); 							//the right bucket gainlist

	//pseudo random seperate the cell into two partition
	partition =  init_partition(&leftPartitionCellCount, &rightPartitionCellCount);
	printPartition(partition, leftPartitionCellCount, rightPartitionCellCount);

	//calculate the gain of all if the cell
	gain = calculate_all_gain(netArray, cellArray, partition);

	//build the bucketlist data structure
	BuildGainList(gain, LeftGainList, RightGainList, partition);

	//initialize the cell statis to unlock (unlock, lock) = (0, 1)
	CellLockState = BuildCellLockState();


	int move_cell_id;
	bool comeFrom;
	vector <int> best_partition;
	best_partition.assign(partition.begin(), partition.end());
	int left_best_partition_count = leftPartitionCellCount;
	int right_best_partition_count = rightPartitionCellCount;
	int totalGain = 0,currentMaxGain=0;
	while(1){
		
		for(int i = 1; i <= nodeNumber / 2; i++){
			move_cell_id = getMaxGainCell(LeftGainList, RightGainList, &comeFrom, leftPartitionCellCount, rightPartitionCellCount);
			totalGain += gain[move_cell_id];

			removeFromBucketList(move_cell_id, comeFrom, LeftGainList, RightGainList, gain);
			updateLockState(move_cell_id, CellLockState);
			updatePartition(move_cell_id, partition, &leftPartitionCellCount, &rightPartitionCellCount, comeFrom);

			//to decide whether update the best partition due to total gain
			if(totalGain > currentMaxGain){
				currentMaxGain = totalGain;
				best_partition.assign(partition.begin(), partition.end());
				left_best_partition_count = leftPartitionCellCount;
				right_best_partition_count = rightPartitionCellCount;
				//printPartition(partition, leftPartitionCellCount, rightPartitionCellCount);
			}
			updateNeighborGain(LeftGainList, RightGainList, netArray, cellArray, gain, move_cell_id, partition, CellLockState);
			gain[move_cell_id] = -1 * gain[move_cell_id];
		}
		totalGain = currentMaxGain;
		leftPartitionCellCount = left_best_partition_count;
		rightPartitionCellCount = right_best_partition_count;
		partition.assign(best_partition.begin(), best_partition.end());
		unlockClockState(CellLockState);
		BuildGainList(gain, LeftGainList, RightGainList, partition);

		//get the current time, and determine whether to output the current best partition or not
		if(omp_get_wtime() - start_time >= CUT_OFF_TIME) break;	
	}

	//output the heuristic partition
	outputBestPartition(best_partition);
	return 0;
}