
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
#include <iostream>
#include <string>
#include <vector>
#include "GetLongOpt.h"
#include "kai_gate.h"
#include "kai_netlist.h"
#include "kai_path.h"
#include "kai_constraint.h"
#include "kai_objective.h"
// Lu's Solver
#include "csat_new1/csat_netlist.h"
#include "csat_new1/csat_const.h"

extern KaiNetlist CUT;
extern SNetlist LuCUT;

void ConvertToSNetlist()
{
	cout<<"Covert To Lu Feng's Format..."<<endl;	
	LuCUT._name=CUT.GetName();
	LuCUT.init();
	
	KaiGATE *kai, *kaiIN;
	SGate *lu, *luIN;

	// Convert
	for(unsigned int IdxG=0; IdxG<CUT.NoGate(); IdxG++) {
		kai=CUT.Gate(IdxG);
		lu=new SGate();
		if(!lu) { cerr<<"No Memory !"<<endl; exit(-1); }
		lu->_name=kai->GetName();
		if(kai->GetFunction()==G_PI) { lu->_type=CSAT_BPI; LuCUT._vpi.push_back(lu); }
		else if(kai->GetFunction()==G_AND) { lu->_type=CSAT_BAND; LuCUT._vgate.push_back(lu); }
		else if(kai->GetFunction()==G_NAND) { lu->_type=CSAT_BNAND; LuCUT._vgate.push_back(lu); }
		else if(kai->GetFunction()==G_OR) { lu->_type=CSAT_BOR; LuCUT._vgate.push_back(lu); }
		else if(kai->GetFunction()==G_NOR) { lu->_type=CSAT_BNOR; LuCUT._vgate.push_back(lu); }
		else if(kai->GetFunction()==G_XOR) { lu->_type=CSAT_BXOR; LuCUT._vgate.push_back(lu); }
		else if(kai->GetFunction()==G_NOT) { lu->_type=CSAT_BINV; LuCUT._vgate.push_back(lu); }
		else if(kai->GetFunction()==G_BUF) { lu->_type=CSAT_BBUF; LuCUT._vgate.push_back(lu); }
		else if(kai->GetFunction()==G_PO) { lu->_type=CSAT_BPO; LuCUT._vpo.push_back(lu); }
		else if(kai->GetFunction()==G_CNF) { lu->_type=CSAT_BCNF; LuCUT._vconstrain.push_back(lu); }
		else { cerr<<"Wrong Type in ConvertToSNetlist() :"<<kai->GetName()<<endl; exit(-1); }
		for(unsigned int IdxS=0; IdxS<kai->NoFaninSign(); ++IdxS) {
			lu->_vin_sign.push_back(kai->FaninSign(IdxS));
		}
		if(LuCUT._name_table(lu->_name)!=NULL) { cerr<<"Duplicate Name "<<lu->_name<<endl; exit(-1); }
		LuCUT._name_table[lu->_name]=lu;
	}
	// Build Connection
	for(unsigned int IdxG=0; IdxG<CUT.NoGate(); IdxG++) {
		kai=CUT.Gate(IdxG);
		lu=LuCUT._name_table(kai->GetName());
		if(!lu) { cerr<<"Miss Kai Name"<<kai->GetName()<<endl; exit(-1); }
		for(unsigned int i=0; i<kai->NoFanin(); ++i) {
			kaiIN=kai->Fanin(i);
			luIN=LuCUT._name_table(kaiIN->GetName());
			if(!luIN) { cerr<<"Miss KaiIn Name"<<kaiIN->GetName()<<endl; exit(-1); }
			lu->_vin.push_back(luIN);
			luIN->_vout.push_back(lu);
		}
	}

	// Const Gate (If Not Specify in *.bench)
	SGate *lugnd;
	lugnd=new SGate();
	if(!lugnd) { cerr<<"No Memory !"<<endl; exit(-1); }
	lugnd->_name="gnd";
	lugnd->_type=CSAT_CONST;
	LuCUT._vconst.push_back(lugnd);
	for(unsigned int IdxG=0; IdxG<CUT.NoGate(); IdxG++) {
		kai=CUT.Gate(IdxG);
		lu=LuCUT._name_table(kai->GetName());
		if(kai->GetFunction()==G_BUF&&kai->GetGND()) {
			assert(!kai->GetVDD());
			assert(lu->_vin.size()==0);
			lu->_vin.push_back(lugnd);
			lu->_vin_sign.push_back(false);
			lugnd->_vout.push_back(lu);
		}
	}

	SGate *luvdd;
	luvdd=new SGate();
	if(!luvdd) { cerr<<"No Memory !"<<endl; exit(-1); }
	luvdd->_name="vdd";
	luvdd->_type=CSAT_CONST;
	LuCUT._vconst.push_back(luvdd);
	for(unsigned int IdxG=0; IdxG<CUT.NoGate(); IdxG++) {
		kai=CUT.Gate(IdxG);
		lu=LuCUT._name_table(kai->GetName());
		if(kai->GetFunction()==G_BUF&&kai->GetVDD()) {
			assert(!kai->GetGND());
			assert(lu->_vin.size()==0);
			lu->_vin.push_back(luvdd);
			lu->_vin_sign.push_back(false);
			luvdd->_vout.push_back(lu);
		}
	}
	//cout<<LuCUT;
}
