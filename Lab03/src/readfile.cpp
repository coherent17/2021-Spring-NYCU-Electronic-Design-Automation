#include "readfile.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <vector>
#include <map>

using namespace std;
using std::vector;
using std::map;
using std::pair;

void readFile(char *filename, RawData *rawdata){
	char buffer[BUFFER_SIZE];

	//read the expression of the function
	FILE *input = fopen(filename, "r");
	fscanf(input, "%s", buffer);
	printf("Expression: %s\n", buffer);
	vector <vector<Variable>> temp_Literals;
	char *token = strtok(buffer, "+");
	while(token!=NULL){
		vector<Variable> Literal;
		for(int i = 0; i < (int)strlen(token); i++){
			if(token[i] == '.') break;
			Variable temp;
			if(isupper(token[i])){
				temp.ASCII_id = int(token[i] - 'A');
				temp.isNegative = true;
				Literal.emplace_back(temp);
			}
			else if(islower(token[i])){
				temp.ASCII_id = int(token[i] - 'a');
				temp.isNegative = false;
				Literal.emplace_back(temp);
			}
		}
		temp_Literals.emplace_back(Literal);
		token = strtok(NULL, "+");
	}
	rawdata->Literals = temp_Literals;

	//read the order of the variable
	vector <vector<int>> VarOrder;
	vector <map <int, int>> VarIndex;
	while(fscanf(input, "%s", buffer)!=EOF){
		vector <int> OneOrder;
		map <int, int> temp_map;
		for(int i = 0; i < (int)(strlen(buffer) - 1); i++){
			OneOrder.emplace_back(int(buffer[i] - 'a'));
			temp_map.insert(pair<int, int>(int(buffer[i] - 'a'), i));
		}
		VarOrder.emplace_back(OneOrder);
		VarIndex.emplace_back(temp_map);
	}
	rawdata->VarOrder = VarOrder;
	rawdata->VarIndex = VarIndex;
	fclose(input);
}

void printRawData(RawData rawdata){
	printf("Literals:\n");
	for(int i = 0; i < (int)rawdata.Literals.size(); i++){
		printf("%dth literal:\n", i);
		for(int j = 0; j < (int)rawdata.Literals[i].size(); j++){
			printf("\treference ASCII: %d, isNegative: %d\n", rawdata.Literals[i][j].ASCII_id, rawdata.Literals[i][j].isNegative);
		}
	}

	printf("VarOrder:\n");
	for(int i = 0; i < (int)rawdata.VarOrder.size(); i++){
		for(int j = 0; j < (int)rawdata.VarOrder[i].size(); j++){
			printf("%d ", rawdata.VarOrder[i][j]);
		}
		printf("\n");
	}
	printf("\n");

	printf("VarIndex\n");
	for(int i = 0; i < (int)rawdata.VarIndex.size(); i++){
		for(const auto& s : rawdata.VarIndex[i]){
			printf("reference Ascii: %d at index = %d\n", s.first, s.second);
	    }
	    printf("\n");
	}
}