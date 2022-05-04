#include <stdio.h>
#include <stdlib.h>
#include "cudd.h"
#include "util.h"
#include "config.h"

void write_dd(DdManager *gbm, DdNode *dd, char *filename){
	FILE *outfile = fopen(filename, "w");
	DdNode **ddnodearray = (DdNode **)malloc(sizeof(DdNode *));
	ddnodearray[0] = dd;
	Cudd_DumpDot(gbm, 1, ddnodearray, NULL, NULL, outfile);
	free(ddnodearray);
	fclose(outfile);
}

int main(){
	char filename[30];
	DdManager *gbm;
	gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
	DdNode *bdd, *x1, *x2;
	x1 = Cudd_bddNewVar(gbm);
	x2 = Cudd_bddNewVar(gbm);
	bdd = Cudd_bddAnd(gbm, x1, x2);
	Cudd_Ref(bdd);

	bdd = Cudd_BddToAdd(gbm, bdd);
	//sprintf(filename, "./bdd/graph.dot");
	printf("NodeCount = %d", Cudd_ReadSize(gbm));
	write_dd(gbm, bdd, filename);
	Cudd_Quit(gbm);
	return 0;
}