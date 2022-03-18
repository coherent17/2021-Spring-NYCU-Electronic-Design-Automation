#include "FiducciaMattheysesAlgorithm.h"
#include <stdbool.h>
#include <vector>
#include <unordered_set>

#define AREA_LOW_BLANCED_FACTOR 0.45
#define AREA_HIGH_BLANCED_FACTOR 0.55
#define FIND_CELL_ERROR -1

using namespace std;
using std::vector;
using std::unordered_set;

double calculate_balanced_factor(int leftPartitionCellCount, int rightPartitionCellCount, int numerator){
	return (double)(numerator) / (double)(leftPartitionCellCount + rightPartitionCellCount);
}

int getMaxGainCell(vector<unordered_set<int>> &leftGainList, vector<unordered_set<int>> &rightGainList, bool *comeFrom, int max_terminal, int leftPartitionCellCount, int rightPartitionCellCount){
	int returnCellid;

	double LeftBalanceFactor = calculate_balanced_factor(leftPartitionCellCount - 1, rightPartitionCellCount + 1, leftPartitionCellCount - 1);
	double RightBalanceFactor = calculate_balanced_factor(leftPartitionCellCount + 1, rightPartitionCellCount - 1, leftPartitionCellCount + 1);
	
	//the condition that must move the right cell to the left partition
	if(LeftBalanceFactor <= AREA_LOW_BLANCED_FACTOR){
		for(int i = max_terminal * 2; i >= 0 ;i--){
			if(!rightGainList[i].empty()){
				printf("here\n");
				*(comeFrom) = 1;
				returnCellid = *(rightGainList[i].begin());
				return returnCellid;
			}
		}
	}

	//the condition that must move the left cell to the right partition
	else if(RightBalanceFactor <= AREA_LOW_BLANCED_FACTOR){
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
	return FIND_CELL_ERROR;
}

/*
vector <bool> Move(vector<unordered_set<int>> &leftGainList, vector<unordered_set<int>> &rightGainList, vector <bool> &partition, vector<unordered_set<int>> &netArray, vector<unordered_set<int>> &cellArray, vector <bool> &CellLockState, int nodeNumber, int *leftPartitionCellCount, int *rightPartitionCellCount){
	int rest_node = nodeNumber;
	vector <bool> currentPartition.assign = (partition.begin(), partition.end());
	while(rest_node > 1){

	}

}
*/