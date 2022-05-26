
/* =========FOR INTERNAL USE ONLY. NO DISTRIBUTION PLEASE ========== */

/*********************************************************************
 Copyright 2002-2006, University of California at Santa Barbara (UCSB).  
 All rights reserved. 

 --- This software and any associated documentation is provided "as is" 

 UCSB and the authors MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS 
 OR IMPLIED, INCLUDING THOSE OF MERCHANTABILITY OR FITNESS FOR A 
 PARTICULAR PURPOSE, OR THAT  USE OF THE SOFTWARE, MODIFICATIONS, OR 
 ASSOCIATED DOCUMENTATION WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, 
 TRADEMARKS OR OTHER INTELLECTUAL PROPERTY RIGHTS OF A THIRD PARTY.  

 UCSB and the authors shall not be liable under any circumstances for 
 any direct, indirect, special, incidental, or consequential damages 
 with respect to any claim by USER or any third party on account of 
 or arising from the use, or inability to use, this software or its 
 associated documentation, even if UCSB and/or the authors has been advised
 of the possibility of those damages.
*********************************************************************/
// Author: Kai Yang (kyang@ece.ucsb.edu)

using namespace std;

#include <string.h>
#include <cstdlib>
#include <assert.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "GetLongOpt.h"
#include "kai_gate.h"
#include "kai_path.h"
#include "kai_objective.h"
#include "kai_typeemu.h"

extern GetLongOpt option;
extern map<string, KaiGATE*> CUTNameTable;

string ToCUTName(KaiGATE *gptr, int TF)
{
	char tmp[100];
	sprintf(tmp, "%d", TF);
	string dupname;
	if(gptr->GetFunction()!=G_DFF)
		dupname=gptr->GetName()+"_dup_t"+tmp;
	else
		dupname=gptr->GetName()+"_ppi_t"+tmp;
	KaiGATE *tg=CUTNameTable[dupname];
	if(!tg) { cerr<<"Miss ToCUTName()"<<endl; exit(-1); }
	return dupname;
}

void AtpgObj::BuildFromPath_NR(PATH *pptr)
{
	cleanup();
	KaiGATE *CurG, *PreG;
	TRANSITION CurT, PreT;
	assert(pptr->NoGate() == pptr->NoTrans());

	//get the first gate and fist transition
	PreG = pptr->GetGate(0);	/* input gate on sensitive path*/
	PreT = pptr->GetTrans(0);	/* input transition on sensitive path*/

	//define the transition state (R : rising & F : falling)


	// Fault Activation at 1st TimeFrame
	if(PreT==R) AddObj(ToCUTName(PreG, 0), 0);
	else if(PreT==F) AddObj(ToCUTName(PreG, 0), 1);
	else { cerr<<"R/F Error !"<<endl; exit(-1); }

	// Fault Activation at 2nd TimeFrame 
	if(PreT==R) AddObj(ToCUTName(PreG, 1), 1);
	else if(PreT==F) AddObj(ToCUTName(PreG, 1), 0);
	else { cerr<<"R/F Error !"<<endl; exit(-1); }

	/*Fault Propagation = off-input setting on sensitive path */
	for(int i = 1; i < pptr->NoGate(); i++){
		CurG = pptr->GetGate(i);
		CurT = pptr->GetTrans(i);

		//enum GATEFUNC { G_PI=0, G_PO, G_NOT, G_AND, G_NAND, G_OR, G_NOR, G_DFF, G_BUF, G_XOR, G_BAD, G_CNF };
		//                  0       1     2      3      4       5     6      7      8      9      10	 11

		//five options for the gate: G_PI, G_AND, G_NAND, G_OR, G_NOR

		switch(CurG->GetFunction()){

			//Primary Input Gate:
			case G_PI:
				//1st TimeFrame
				if (CurT == R) AddObj(ToCUTName(CurG, 0), 0);
      			else if (CurT == F) AddObj(ToCUTName(CurG, 0), 1);
      			else { cerr<<"R/F Error !"<<endl; exit(-1); }

      			//2nd TimeFrame
      			if(CurT==R) AddObj(ToCUTName(CurG, 1), 1);
				else if(CurT==F) AddObj(ToCUTName(CurG, 1), 0);
				else { cerr<<"R/F Error !"<<endl; exit(-1); }
				break;

			//And Gate: cv = 0, ncv = 1, rising: x->1, falling: x->1
			case G_AND:

				//rising: x->1
				if (PreT == R){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 1), 1);
						}
					}
				}

				//falling: x->1
				else if (PreT == F){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 1), 1);
						}
					}					
				}

				else { cerr<<"R/F Error !"<<endl; exit(-1); }
				break;


			//Nand Gate: cv = 0, ncv = 1, rising: x->1, falling: x->1
			case G_NAND:

				//rising: x->1
				if (PreT == R){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 1), 1);
						}
					}
				}

				//falling: x->1
				else if (PreT == F){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 1), 1);
						}
					}					
				}

				else { cerr<<"R/F Error !"<<endl; exit(-1); }
				break;

			//Or Gate: cv = 1, ncv = 0, rising: x->0, falling: x->0
			case G_OR:

				//rising: x->0
				if (PreT == R){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 1), 0);
						}
					}
				}

				//falling: x->0
				else if (PreT == F){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 1), 0);
						}
					}					
				}

				else { cerr<<"R/F Error !"<<endl; exit(-1); }
				break;

			//Nor Gate: cv = 1, ncv = 0, rising: x->0, falling: x->0
			case G_NOR:

				//rising: x->0
				if (PreT == R){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 1), 0);
						}
					}
				}

				//falling: x->0
				else if (PreT == F){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 1), 0);
						}
					}					
				}

				else { cerr<<"R/F Error !"<<endl; exit(-1); }
				break;

			default:
				break;
		}
		PreG = CurG;
		PreT = CurT;
	}
}

void AtpgObj::BuildFromPath_R(PATH *pptr)
{
	cleanup();
	KaiGATE *CurG, *PreG;
	TRANSITION CurT, PreT;
	assert(pptr->NoGate()==pptr->NoTrans());
    
	/*Do Fault Activation & Fault Propagation under Robust test setting*/
	
	//get the first gate and fist transition
	PreG = pptr->GetGate(0);
	PreT = pptr->GetTrans(0);

	// Fault Activation at 1st TimeFrame
	if(PreT==R) AddObj(ToCUTName(PreG, 0), 0);
	else if(PreT==F) AddObj(ToCUTName(PreG, 0), 1);
	else { cerr<<"R/F Error !"<<endl; exit(-1); }

	// Fault Activation at 2nd TimeFrame 
	if(PreT==R) AddObj(ToCUTName(PreG, 1), 1);
	else if(PreT==F) AddObj(ToCUTName(PreG, 1), 0);
	else { cerr<<"R/F Error !"<<endl; exit(-1); }

	for(int i = 1; i < pptr->NoGate(); i++){
		CurG = pptr->GetGate(i);
    	CurT = pptr->GetTrans(i);
    	switch(CurG->GetFunction()){

			//Primary Input Gate:
			case G_PI:
				//1st TimeFrame
				if (CurT == R) AddObj(ToCUTName(CurG, 0), 0);
      			else if (CurT == F) AddObj(ToCUTName(CurG, 0), 1);
      			else { cerr<<"R/F Error !"<<endl; exit(-1); }

      			//2nd TimeFrame
      			if(CurT==R) AddObj(ToCUTName(CurG, 1), 1);
				else if(CurT==F) AddObj(ToCUTName(CurG, 1), 0);
				else { cerr<<"R/F Error !"<<endl; exit(-1); }
				break;

			//And Gate: cv = 0, ncv = 1, rising: x->1, falling: 1->1
			case G_AND:

				//rising: x->1
				if (PreT == R){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 1), 1);
						}
					}
				}

				//falling: x->1
				else if (PreT == F){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 0), 1);
							AddObj(ToCUTName(CurG->Fanin(j), 1), 1);
						}
					}					
				}

				else { cerr<<"R/F Error !"<<endl; exit(-1); }
				break;


			//Nand Gate: cv = 0, ncv = 1, rising: x->1, falling: 1->1
			case G_NAND:

				//rising: x->1
				if (PreT == R){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 1), 1);
						}
					}
				}

				//falling: 1->1
				else if (PreT == F){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 0), 1);
							AddObj(ToCUTName(CurG->Fanin(j), 1), 1);
						}
					}					
				}

				else { cerr<<"R/F Error !"<<endl; exit(-1); }
				break;

			//Or Gate: cv = 1, ncv = 0, rising: 0->0, falling: x->0
			case G_OR:

				//rising: 0->0
				if (PreT == R){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 0), 0);
							AddObj(ToCUTName(CurG->Fanin(j), 1), 0);
						}
					}
				}

				//falling: x->0
				else if (PreT == F){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 1), 0);
						}
					}					
				}

				else { cerr<<"R/F Error !"<<endl; exit(-1); }
				break;

			//Nor Gate: cv = 1, ncv = 0, rising: 0->0, falling: x->0
			case G_NOR:

				//rising: x->0
				if (PreT == R){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 0), 0);
							AddObj(ToCUTName(CurG->Fanin(j), 1), 0);
						}
					}
				}

				//falling: x->0
				else if (PreT == F){
					//setting all off-inputs to non-controlling value (ncv = 1)
					for(int j = 0; j < CurG->NoFanin(); j++){

						//just to make sure skip the on-input fanin (controlling value)
						if(PreG != CurG->Fanin(j)){
							AddObj(ToCUTName(CurG->Fanin(j), 1), 0);
						}
					}					
				}

				else { cerr<<"R/F Error !"<<endl; exit(-1); }
				break;

			default:
				break;
		}
		PreG = CurG;
		PreT = CurT;
	}
}


void AtpgObj::Dump()
{
	assert(_name.size()==_value.size());
	for(unsigned int i=0; i<_name.size(); ++i) {
		cout<<_name[i]<<"("<<_value[i]<<") ";
	}
	cout<<endl;
}
