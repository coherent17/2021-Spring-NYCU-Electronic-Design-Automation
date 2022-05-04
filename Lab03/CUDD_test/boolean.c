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
	char filename[] = "boolean_out";
	DdManager *gbm;
	gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
	DdNode *bdd, *var, *tmp_neg, *tmp;
	int i;
	bdd = Cudd_ReadOne(gbm);
	Cudd_Ref(bdd);

	for(i = 3; i >= 0; i--){
		var = Cudd_bddIthVar(gbm, i);
		tmp_neg = Cudd_Not(var);
		tmp = Cudd_bddAnd(gbm, tmp_neg, bdd);
		Cudd_Ref(tmp);
		Cudd_RecursiveDeref(gbm, bdd);
		bdd = tmp;
	}

	bdd = Cudd_BddToAdd(gbm, bdd);
	printf("NodeCount = %d", Cudd_ReadSize(gbm));
	write_dd(gbm, bdd, filename);
	Cudd_Quit(gbm);
	return 0;
}