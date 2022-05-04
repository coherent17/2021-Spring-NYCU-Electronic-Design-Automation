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
	DdNode *bdd1, *bdd2, *a, *b, *c, *d, *bdd;
	a = Cudd_bddNewVar(gbm);
	b = Cudd_bddNewVar(gbm);
	c = Cudd_bddNewVar(gbm);
	d = Cudd_bddNewVar(gbm);
	bdd1 = Cudd_bddAnd(gbm, a, b);
	bdd2 = Cudd_bddAnd(gbm, c, d);
	bdd = Cudd_bddOr(gbm, bdd1, bdd2);
	Cudd_Ref(bdd);

	printf("NodeCount = %ld\n", Cudd_ReadNodeCount(gbm));
	printf("var = %d\n", Cudd_ReadSize(gbm));

	bdd = Cudd_BddToAdd(gbm, bdd);
	printf("NodeCount = %ld\n", Cudd_ReadNodeCount(gbm));
	printf("var = %d\n", Cudd_ReadSize(gbm));

	sprintf(filename, "../cudd-3.0.0/bdd/graph.dot");
	write_dd(gbm, bdd, filename);
	Cudd_Quit(gbm);
	return 0;
}