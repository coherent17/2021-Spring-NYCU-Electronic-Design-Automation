
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
	assert(pptr->NoGate()==pptr->NoTrans());

	PreG=/* input gate on sensitive path*/
	PreT=/* input transition on sensitive path*/
	
	// Fault Activation at 1st TimeFrame
	if(PreT==R) AddObj(ToCUTName(PreG, 0), /*value*/);
	else if(PreT==F) AddObj(ToCUTName(PreG, 0), /*value*/);
	else { cerr<<"R/F Error !"<<endl; exit(-1); }

	// Fault Activation at 2nd TimeFrame 
	if(PreT==R) AddObj(ToCUTName(PreG, 1), /*value*/);
	else if(PreT==F) AddObj(ToCUTName(PreG, 1), /*value*/);
	else { cerr<<"R/F Error !"<<endl; exit(-1); }
	
   /*Fault Propagation = off-input setting on sensitive path */

	
}

void AtpgObj::BuildFromPath_R(PATH *pptr)
{
	cleanup();
	KaiGATE *CurG, *PreG;
	TRANSITION CurT, PreT;
	assert(pptr->NoGate()==pptr->NoTrans());
    
	/*Do Fault Activation & Fault Propagation under Robust test setting*/

}


void AtpgObj::Dump()
{
	assert(_name.size()==_value.size());
	for(unsigned int i=0; i<_name.size(); ++i) {
		cout<<_name[i]<<"("<<_value[i]<<") ";
	}
	cout<<endl;
}
