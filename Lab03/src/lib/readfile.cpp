#include "readfile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>

using namespace std;
using std::vector;

void readFile(char *filename, RawData *rawdata){
	char buffer[BUFFER_SIZE];

	//read the expression of the function
	FILE *input = fopen(filename, "r");
	fscanf(input, "%s", buffer);
	strncpy(rawdata->expression, buffer, strlen(buffer) - 1);

	//read the order of the variable
	vector <vector<int>> VarOrder;
	while(fscanf(input, "%s", buffer)!=EOF){
		vector <int> OneOrder;
		for(int i = 0; i < (int)(strlen(buffer) - 1); i++){
			OneOrder.emplace_back(int(buffer[i]));
		}
		VarOrder.emplace_back(OneOrder);
	}
	rawdata->VarOrder = VarOrder;
	fclose(input);
}

void printRawData(RawData rawdata){
	printf("Expression: %s\n", rawdata.expression);

	for(int i = 0; i < (int)rawdata.VarOrder.size(); i++){
		for(int j = 0; j < (int)rawdata.VarOrder[i].size(); j++){
			printf("%d ", rawdata.VarOrder[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}