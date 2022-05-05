#include <stdio.h>
#include <stdlib.h>
#include "cudd.h"
#include "util.h"
#include "config.h"

#include "buildBDD.h"
#include "readfile.h"

extern DdManager *gbm;
extern DdNode *bdd;

void buildBDD(RawData rawdata, int whichOrder){
	gbm = Cudd_Init(0, 0, CUDD_UNIQUE_SLOTS, CUDD_CACHE_SLOTS, 0);
	DdNode *temp;
	bdd = Cudd_ReadLogicZero(gbm);
	Cudd_Ref(bdd);

	for(const auto Literal : rawdata.Literals){
		DdNode *term = Cudd_ReadOne(gbm);
		Cudd_Ref(term);

		for(const auto Variable : Literal){
			DdNode *Node;
			if(!Variable.isNegative){
				Node = Cudd_bddIthVar(gbm, rawdata.VarIndex[whichOrder][Variable.ASCII_id]);
			}
			else if(Variable.isNegative){
				Node = Cudd_Not(Cudd_bddIthVar(gbm, rawdata.VarIndex[whichOrder][Variable.ASCII_id]));
			}
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