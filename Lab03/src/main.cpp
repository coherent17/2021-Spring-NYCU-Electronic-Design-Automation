#include <stdio.h>
#include <stdlib.h>
#include "cudd.h"
#include "util.h"
#include "config.h"

#include "readfile.h"
#include "buildBDD.h"

//extern global variable
DdManager *gbm;
DdNode *bdd;

void write_dd(DdManager *gbm, DdNode *dd){
	FILE *outfile = fopen("test", "w");
	DdNode **ddnodearray = (DdNode **)malloc(sizeof(DdNode *));
	ddnodearray[0] = dd;
	Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile);
	free(ddnodearray);
	fclose(outfile);
}

int main(int argc, char *argv[]){
	char *inputName = *(argv + 1);
	//char *outputName = *(argv + 2);

	RawData rawdata;
	
	//read file
	readFile(inputName, &rawdata);
	printRawData(rawdata);

	//build bbd
	void buildBDD(RawData rawdata);
	bdd = Cudd_BddToAdd(gbm, bdd);
	write_dd(gbm, bdd);
	Cudd_Quit(gbm);
	return 0;
}