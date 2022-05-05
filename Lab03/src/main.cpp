#include <stdio.h>
#include <stdlib.h>
#include "cudd.h"
#include "util.h"
#include "config.h"

#include "readfile.h"
#include "buildBDD.h"

#define INT_MAX 2147483647

//extern global variable
DdManager *gbm;
DdNode *bdd;

void write_dd(DdManager *gbm, DdNode *dd, char *filename){
	FILE *outfile = fopen(filename, "w");
	DdNode **ddnodearray = (DdNode **)malloc(sizeof(DdNode *));
	ddnodearray[0] = dd;
	Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile);
	free(ddnodearray);
	fclose(outfile);
}

int main(int argc, char *argv[]){
	char *inputName = *(argv + 1);
	char *outputName = *(argv + 2);

	RawData rawdata;
	
	//read file
	readFile(inputName, &rawdata);
	//printRawData(rawdata);

	int minNode = INT_MAX;

	//build bbd in different reduce order
	for(int i = 0; i < (int)rawdata.VarOrder.size(); i++){
		buildBDD(rawdata, i);
		bdd = Cudd_BddToAdd(gbm, bdd);
		char filename[30];
		sprintf(filename, "%d_order.dot", i+1);
		write_dd(gbm, bdd, filename);
		int temp = Cudd_DagSize(bdd);
		minNode = (minNode > temp) ? temp : minNode;
		Cudd_Quit(gbm);		
	}

	//output the answer:
	FILE *output = fopen(outputName, "w");
	fprintf(output,"%d\n", minNode);
	fclose(output);
	return 0;
}