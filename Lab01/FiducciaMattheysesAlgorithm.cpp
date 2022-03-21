#include "FiducciaMattheysesAlgorithm.h"
#include "GainList.h"
#include <vector>
#include <unordered_set>

#define AREA_LOW_BLANCED_FACTOR 0.45
#define FIND_CELL_ERROR -1

using namespace std;
using std::vector;
using std::unordered_set;

extern int netNumber;
extern int nodeNumber;
extern int max_terminal;

typedef struct vector<unordered_set<int>> vu;

int getMaxGainCell(vu &leftGainList, vu &rightGainList, bool *comeFrom, int leftPartitionCellCount, int rightPartitionCellCount){
	int returnCellid;
	//printf("leftPartitionCellCount = %d, rightPartitionCellCount = %d\n", leftPartitionCellCount, rightPartitionCellCount);
	//the condition that must move the right cell to the left partition
	if(leftPartitionCellCount - 1 <= AREA_LOW_BLANCED_FACTOR * (leftPartitionCellCount + rightPartitionCellCount) - 1){
		for(int i = max_terminal * 2; i >= 0 ;i--){
			if(!rightGainList[i].empty()){
				*(comeFrom) = 1;
				returnCellid = *(rightGainList[i].begin());
				return returnCellid;
			}
		}
	}

	//the condition that must move the left cell to the right partition
	else if(rightPartitionCellCount - 1 <= AREA_LOW_BLANCED_FACTOR * (leftPartitionCellCount + rightPartitionCellCount) - 1){
		for(int i = max_terminal * 2; i >= 0; i--){
			if(!leftGainList[i].empty()){
				*(comeFrom) = 0;
				returnCellid = *(leftGainList[i].begin());
				return returnCellid;
			}			
		}
	}

	//move which cell will not violate the area constraint
	else{
		for(int i = max_terminal * 2; i >= 0; i--){

			//if both the gain list are empty for this gain value
			if(leftGainList[i].empty() && rightGainList[i].empty()) continue;

			//if both the gain list isn't empty, choose the left side first
			else if(!leftGainList[i].empty() && !rightGainList[i].empty()){
				*(comeFrom) = 0;
				returnCellid = *(leftGainList[i].begin());
				return returnCellid;
			}

			//if the gainList on the left side is not empty, and the right side is empty
			else if(!leftGainList[i].empty() && rightGainList[i].empty()){
				*(comeFrom) = 0;
				returnCellid = *(leftGainList[i].begin());
				return returnCellid;				
			}

			//if the gainList on the left side is empty, and the right side isn't empty
			else if(leftGainList[i].empty() && !rightGainList[i].empty()){
				*(comeFrom) = 1;
				returnCellid = *(rightGainList[i].begin());
				return returnCellid;					
			}

		}
	}
	(*comeFrom) = 0;
	printf("Find Cell Error!\n");
	printf("Find Cell Error!\n");
	printf("Find Cell Error!\n");
	printf("Find Cell Error!\n");
	printf("Find Cell Error!\n");
	printf("Find Cell Error!\n");
	printf("Find Cell Error!\n");
	printf("Find Cell Error!\n");
	return FIND_CELL_ERROR;
}

void removeFromBucketList(int move_cell_id, bool comeFrom, vu &leftGainList, vu &rightGainList, vector <int> &gain){
	int removeGainIndex = gain[move_cell_id] + max_terminal;

	//remove the cell from the left bucketlist
	if(comeFrom == 0){
		leftGainList[removeGainIndex].erase(move_cell_id);
		return;
	}
	else{
		rightGainList[removeGainIndex].erase(move_cell_id);
		return;
	} 
	printf("Delete %d error!\n", move_cell_id);
}

void updateLockState(int move_cell_id, vector <int> &CellLockState){
	CellLockState[move_cell_id] = 1;
	return;
}

void updatePartition(int move_cell_id, vector <int> &partition, int *leftPartitionCellCount, int *rightPartitionCellCount, bool comeFrom){
	//if the cell come from left bucket list to right bucketlist
	if(comeFrom == 0){
		partition[move_cell_id] = 1;
		(*leftPartitionCellCount)--;
		(*rightPartitionCellCount)++;
		return;
	}
	//if the cell come from right bucket list to left bucketlist
	else if(comeFrom == 1){
		partition[move_cell_id] = 0;
		(*leftPartitionCellCount)++;
		(*rightPartitionCellCount)--;
		return;
	}
	return;
}

void updateNeighborGain(vu &leftGainList, vu &rightGainList, vu &netArray, vu &cellArray, vector <int> &gain, int move_cell_id, vector <int> &partition, vector <int> &CellLockState){
	//to loop through the net that the target cell connect with, for each net, check for the unlock neighbor to update the gain
	for(const auto &i : cellArray[move_cell_id]){
		for(const auto &j : netArray[i]){
			if(j != move_cell_id && CellLockState[j]==0){
				//remove neighbor from the bucketlist
				removeFromBucketList(j, partition[j], leftGainList, rightGainList, gain);
				
				//calculate the gain
				gain[j] = FS(netArray, cellArray, partition, j) - TE(netArray, cellArray, partition, j);

				//insert in the gainlist
				if(partition[j] == 0 && CellLockState[j]==0) leftGainList[gain[j] + max_terminal].insert(j);
				else if(partition[j] == 1 && CellLockState[j]==0) rightGainList[gain[j] + max_terminal].insert(j);
			}
		}
	}
}

void outputBestPartition(vector <int> &best_partition){
	FILE *output = fopen("output.txt", "w");
	for(int i = 1; i <= nodeNumber; i++){
		fprintf(output, "%d\n", (int)best_partition[i]);
	}
	fclose(output);
}