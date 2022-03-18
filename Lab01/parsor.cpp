#include "parsor.h"
#include <vector>
#include <unordered_set>
#include <iostream>
#include <string.h>

using namespace std;
using std::vector;
using std::unordered_set;

vector<unordered_set<int>> BuildNetArray(char *filename, int *netNumber, int *nodeNumber){
	FILE *input = fopen(filename, "r");
	fscanf(input, "%d %d", &(*netNumber), &(*nodeNumber));
	fgetc(input);

	int netArrayindex = 1;
	vector<unordered_set<int>> netArray((*netNumber) + 1);
	char buffer[150000];
	
	while(fscanf(input, "%[^\n]", buffer)!=EOF){	
		char *token = strtok(buffer, " ");
		while(token!=NULL){
			netArray[netArrayindex].insert(atoi(token));
			token = strtok(NULL, " ");
		}
		netArrayindex++;
		fgetc(input);
	}
	fclose(input);
	return netArray;
}

vector<unordered_set<int>> BuildCellArray(char *filename, int *max_terminal){
	FILE *input = fopen(filename, "r");
	int netNumber;
	int nodeNumber;
	fscanf(input, "%d %d", &netNumber, &nodeNumber);
	fgetc(input);

	//the array to record the terminal of each cell
	int *pinCount = (int *)malloc(sizeof(int) * (nodeNumber + 1));	
	for(int i = 0; i <= nodeNumber; i++){
		pinCount[i] = 0;
	}

	vector<unordered_set<int>> cellArray(nodeNumber + 1);

	int netIndex = 1;
	char buffer[150000];
	while(fscanf(input, "%[^\n]", buffer)!=EOF){
		char *token = strtok(buffer, " ");
		while(token!=NULL){
			cellArray[atoi(token)].insert(netIndex);
			pinCount[atoi(token)]++;
			token = strtok(NULL, " ");
		}
		netIndex++;
		fgetc(input);
	}
	fclose(input);
	(*max_terminal) = 0;
	for(int i = 1; i <=nodeNumber; i++){
		(*max_terminal) = (*max_terminal) < pinCount[i] ? pinCount[i] : (*max_terminal);
	}
	free(pinCount);
	return cellArray;
}

void printNetArray(vector<unordered_set<int>> netArray){
	for(int i = 1; i < (int)netArray.size(); i++){
		printf("Net number = %d: ", i);
		for(const auto &s : netArray[i]){
			printf("%d ", s);
		}
		printf("\n");
	}
}

void printCellArray(vector<unordered_set<int>> cellArray){
	for(int i = 1; i < (int)cellArray.size(); i++){
		printf("Cell id = %d: ", i);
		for(const auto &s : cellArray[i]){
			printf("%d ", s);
		}
		printf("\n");
	}
}