#include <stdio.h>
#include <stdlib.h>
#include "cudd.h"
#include "util.h"
#include "config.h"

#include "buildBDD.h"
#include "readfile.h"

extern DdManager *gbm;
extern DdNode *bdd;

void buildBDD(RawData rawdata){
	gbm = Cudd_Init(26, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
	DdNode *temp;
	bdd = Cudd_ReadLogicZero(gbm);
	Cudd_Ref(bdd);
	for(int i = 0; i < (int)rawdata.Literals.size(); i++){
		DdNode *term = Cudd_ReadOne(gbm);
		Cudd_Ref(term);
		for(int j = 0; j < (int)rawdata.Literals[i].size(); j++){
			DdNode *Node = rawdata.Literals[i][j].isNegative ? Cudd_bddIthVar(gbm, rawdata.Literals[i][j].ASCII_id) : Cudd_Not(Cudd_bddIthVar(gbm, rawdata.Literals[i][j].ASCII_id));
			temp = Cudd_bddAnd(gbm, Node, term);
			Cudd_Ref(temp);
			Cudd_RecursiveDeref(gbm, term);
			term = temp;
		}
		temp = Cudd_bddOr(gbm, term, bdd);
		Cudd_Ref(temp);
		Cudd_RecursiveDeref(gbm, bdd);
		bdd = temp;
	}
}