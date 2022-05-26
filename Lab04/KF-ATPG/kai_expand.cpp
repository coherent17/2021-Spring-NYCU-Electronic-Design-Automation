
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

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "GetLongOpt.h"
#include "kai_gate.h"
#include "kai_netlist.h"
#include "kai_path.h"
#include "kai_constraint.h"

extern GetLongOpt option;
extern KaiNetlist NETLIST;
extern KaiNetlist CUT;
extern map<string, KaiGATE*> KaiNameTable;
extern KaiConstraint CNFCONSTRAINT;
map<string, KaiGATE*> CUTNameTable;

void DuplicateCircuit(int);
void BuildConnection(int);
void AddCNFConstraint(int);

int ExpandCircuit()
{
	CUTNameTable.clear();
	int NumTimeFrame=2;
	if((option.retrieve("timeframe")))
		NumTimeFrame=atoi(option.retrieve("timeframe"));
	DuplicateCircuit(NumTimeFrame);
	BuildConnection(NumTimeFrame);
	AddCNFConstraint(NumTimeFrame);
	//CUT.Dump();
	return NumTimeFrame;
}


void DuplicateCircuit(int NumTimeFrame)
{
	if(NumTimeFrame<2) NumTimeFrame=2;
	KaiGATE *gptr;
	string gname;
	KaiGATE *dupptr;
	string dupname;
	char tmp[100];
	
	for(unsigned int IdxG=0; IdxG<NETLIST.NoGate(); ++IdxG) {
		gptr=NETLIST.Gate(IdxG);
		gname=gptr->GetName();
		if(gptr->GetFunction()==G_DFF) {
			for(int i=0; i<NumTimeFrame; ++i) {
				sprintf(tmp, "%d", i);
				// PPI-BUF
				dupname=gname+"_ppi_t"+tmp;
				if(CUTNameTable.find(dupname)!=CUTNameTable.end()) { 
					cerr<<"Error Name Stragegy: "<<dupname<<endl; exit(-1); 
				}
				dupptr=new KaiGATE;
				if(!dupptr) { cerr<<"No Memory !"<<endl; exit(-1); }
				dupptr->SetVDD(gptr->GetVDD());
				dupptr->SetGND(gptr->GetGND());
				dupptr->SetName(dupname);
				CUTNameTable[dupname]=dupptr;
				dupptr->SetFunction(G_BUF);
				CUT.AddGate(dupptr);
				// PPO-BUF
				dupname=gname+"_ppo_t"+tmp;
				if(CUTNameTable.find(dupname)!=CUTNameTable.end()) { 
					cerr<<"Error Name Stragegy: "<<dupname<<endl; exit(-1); 
				}
				dupptr=new KaiGATE;
				if(!dupptr) { cerr<<"No Memory !"<<endl; exit(-1); }
				dupptr->SetVDD(gptr->GetVDD());
				dupptr->SetGND(gptr->GetGND());
				dupptr->SetName(dupname);
				CUTNameTable[dupname]=dupptr;
				dupptr->SetFunction(G_BUF);
				CUT.AddGate(dupptr);
			}
		}
		else {
			for(int i=0; i<NumTimeFrame; ++i) {
				sprintf(tmp, "%d", i);
				dupname=gname+"_dup_t"+tmp;
				if(CUTNameTable.find(dupname)!=CUTNameTable.end()) { 
					cerr<<"Error Name Stragegy: "<<dupname<<endl; exit(-1); 
				}
				dupptr=new KaiGATE;
				if(!dupptr) { cerr<<"No Memory !"<<endl; exit(-1); }
				dupptr->SetVDD(gptr->GetVDD());
				dupptr->SetGND(gptr->GetGND());
				dupptr->SetName(dupname);
				CUTNameTable[dupname]=dupptr;
				dupptr->SetFunction(gptr->GetFunction());
				CUT.AddGate(dupptr);
			}
		}
	}

	// Create PPI / PPO
	for(unsigned int IdxDFF=0; IdxDFF<NETLIST.NoDFF(); ++IdxDFF) {
		gptr=NETLIST.DFFGate(IdxDFF);

		// PPI
		gname=gptr->GetName();
		dupname="PPI_"+gname;
		if(CUTNameTable.find(dupname)!=CUTNameTable.end()) { 
			cerr<<"Error Name Stragegy: "<<dupname<<endl; exit(-1); 
		}
		dupptr=new KaiGATE;
		if(!dupptr) { cerr<<"No Memory !"<<endl; exit(-1); }
		dupptr->SetVDD(gptr->GetVDD());
		dupptr->SetGND(gptr->GetGND());
		dupptr->SetName(dupname);
		CUTNameTable[dupname]=dupptr;
		dupptr->SetFunction(G_PI);
		CUT.AddGate(dupptr);

		// PPO
		assert(gptr->NoFanin()==1);
		gname=gptr->GetName();
		dupname="PPO_"+gname;
		if(CUTNameTable.find(dupname)!=CUTNameTable.end()) { 
			cerr<<"Error Name Stragegy: "<<dupname<<endl; exit(-1); 
		}
		dupptr=new KaiGATE;
		if(!dupptr) { cerr<<"No Memory !"<<endl; exit(-1); }
		dupptr->SetVDD(gptr->GetVDD());
		dupptr->SetGND(gptr->GetGND());
		dupptr->SetName(dupname);
		CUTNameTable[dupname]=dupptr;
		dupptr->SetFunction(G_PO);
		CUT.AddGate(dupptr);

	}
	CUT.InitStructure();
	//CUT.Dump(); exit(1);	
}


void BuildConnection(int NumTimeFrame)
{
	KaiGATE *gptr;
	string gname;
	KaiGATE *dupptr;
	string dupname;
	KaiGATE *tmpptr;
	string tmpname;
	char tmp[100];

	for(unsigned int IdxG=0; IdxG<NETLIST.NoGate(); ++IdxG) {
		gptr=NETLIST.Gate(IdxG);
		gname=gptr->GetName();
		// Original Gates
		if(gptr->GetFunction()!=G_DFF) {
			// Fanin
			for(unsigned int IdxIn=0; IdxIn<gptr->NoFanin(); ++IdxIn) {
				if(gptr->Fanin(IdxIn)->GetFunction()!=G_DFF) {
					for(int IdxTF=0; IdxTF<NumTimeFrame; ++IdxTF) {
						sprintf(tmp, "%d", IdxTF);
						dupname=gname+"_dup_t"+tmp;
						dupptr=CUTNameTable[dupname];
						if(!dupptr) { cerr<<"Miss 1: "<<dupname<<endl; exit(-1); }
						tmpname=gptr->Fanin(IdxIn)->GetName()+"_dup_t"+tmp;
						tmpptr=CUTNameTable[tmpname];
						if(!tmpptr) { cerr<<"Miss 2: "<<tmpname<<endl; exit(-1); }
						dupptr->AddFanin(tmpptr);
					}
				}
				else {
					for(int IdxTF=0; IdxTF<NumTimeFrame; ++IdxTF) {
						sprintf(tmp, "%d", IdxTF);
						dupname=gname+"_dup_t"+tmp;
						dupptr=CUTNameTable[dupname];
						if(!dupptr) { cerr<<"Miss 5: "<<dupname<<endl; exit(-1); }
						tmpname=gptr->Fanin(IdxIn)->GetName()+"_ppi_t"+tmp;
						tmpptr=CUTNameTable[tmpname];
						if(!tmpptr) { cerr<<"Miss 6: "<<tmpname<<endl; exit(-1); }
						dupptr->AddFanin(tmpptr);
						tmpptr->AddFanout(dupptr);
					}
				}
			}
			// Fanout
			for(unsigned int IdxOut=0; IdxOut<gptr->NoFanout(); ++IdxOut) {
				if(gptr->Fanout(IdxOut)->GetFunction()!=G_DFF) {
					for(int IdxTF=0; IdxTF<NumTimeFrame; ++IdxTF) {
						sprintf(tmp, "%d", IdxTF);
						dupname=gname+"_dup_t"+tmp;
						dupptr=CUTNameTable[dupname];
						if(!dupptr) { cerr<<"Miss 7: "<<dupname<<endl; exit(-1); }
						tmpname=gptr->Fanout(IdxOut)->GetName()+"_dup_t"+tmp;
						tmpptr=CUTNameTable[tmpname];
						if(!tmpptr) { cerr<<"Miss 8: "<<tmpname<<endl; exit(-1); }
						dupptr->AddFanout(tmpptr);
					}
				}
				else {
					for(int IdxTF=0; IdxTF<NumTimeFrame; ++IdxTF) {
						sprintf(tmp, "%d", IdxTF);
						dupname=gname+"_dup_t"+tmp;
						dupptr=CUTNameTable[dupname];
						if(!dupptr) { cerr<<"Miss 11: "<<dupname<<endl; exit(-1); }
						tmpname=gptr->Fanout(IdxOut)->GetName()+"_ppo_t"+tmp;
						tmpptr=CUTNameTable[tmpname];
						if(!tmpptr) { cerr<<"Miss 12: "<<tmpname<<endl; exit(-1); }
						dupptr->AddFanout(tmpptr);
						tmpptr->AddFanin(dupptr);
					}
				}
			}
		}
		// DFF
		else {
			assert(gptr->NoFanin()==1);
			
			// Chain-FF (Fanin)
			for(unsigned int IdxIn=0; IdxIn<gptr->NoFanin(); ++IdxIn) {
				if(gptr->Fanin(IdxIn)->GetFunction()==G_DFF) {
					for(int IdxTF=0; IdxTF<NumTimeFrame; ++IdxTF) {
						sprintf(tmp, "%d", IdxTF);
						dupname=gname+"_ppo_t"+tmp;
						dupptr=CUTNameTable[dupname];
						if(!dupptr) { cerr<<"Miss 5: "<<dupname<<endl; exit(-1); }
						tmpname=gptr->Fanin(IdxIn)->GetName()+"_ppi_t"+tmp;
						tmpptr=CUTNameTable[tmpname];
						if(!tmpptr) { cerr<<"Miss 6: "<<tmpname<<endl; exit(-1); }
						dupptr->AddFanin(tmpptr);
					}
				}
			}
			// Chain-FF (Fanout)
			for(unsigned int IdxOut=0; IdxOut<gptr->NoFanout(); ++IdxOut) {
				if(gptr->Fanout(IdxOut)->GetFunction()==G_DFF) {
					for(int IdxTF=0; IdxTF<NumTimeFrame; ++IdxTF) {
						sprintf(tmp, "%d", IdxTF);
						dupname=gname+"_ppi_t"+tmp;
						dupptr=CUTNameTable[dupname];
						if(!dupptr) { cerr<<"Miss 11: "<<dupname<<endl; exit(-1); }
						tmpname=gptr->Fanout(IdxOut)->GetName()+"_ppo_t"+tmp;
						tmpptr=CUTNameTable[tmpname];
						if(!tmpptr) { cerr<<"Miss 12: "<<tmpname<<endl; exit(-1); }
						dupptr->AddFanout(tmpptr);
					}
				}
			}
			
			// TimeFrame 0 : PPI
			dupname=gname+"_ppi_t0";
			dupptr=CUTNameTable[dupname];
			if(!dupptr) { cerr<<"Miss 13: "<<dupname<<endl; exit(-1); }
			tmpname="PPI_"+gname;
			tmpptr=CUTNameTable[tmpname];
			if(!tmpptr) { cerr<<"Miss 14: "<<tmpname<<endl; exit(-1); }
			dupptr->AddFanin(tmpptr);
			tmpptr->AddFanout(dupptr);

			// TimeFrame N-1 : PPO
			sprintf(tmp, "%d", NumTimeFrame-1);
			dupname=gname+"_ppo_t"+tmp;
			dupptr=CUTNameTable[dupname];
			if(!dupptr) { cerr<<"Miss 15: "<<dupname<<endl; exit(-1); }
			tmpname="PPO_"+gptr->GetName();
			tmpptr=CUTNameTable[tmpname];
			if(!tmpptr) { cerr<<"Miss 16: "<<tmpname<<endl; exit(-1); }
			dupptr->AddFanout(tmpptr);
			tmpptr->AddFanin(dupptr);

			// TimeFrame 1 to N-1
			// (Functional Justification)
			for(int IdxTF=0; IdxTF<NumTimeFrame-1; ++IdxTF) {
				sprintf(tmp, "%d", IdxTF);
				dupname=gname+"_ppo_t"+tmp;
				dupptr=CUTNameTable[dupname];
				if(!dupptr) { cerr<<"Miss: "<<dupname<<endl; exit(-1); }
				sprintf(tmp, "%d", IdxTF+1);
				tmpname=gname+"_ppi_t"+tmp;
				tmpptr=CUTNameTable[tmpname];
				if(!tmpptr) { cerr<<"Miss: "<<tmpname<<endl; exit(-1); }
				dupptr->AddFanout(tmpptr);
				tmpptr->AddFanin(dupptr);
			}
		}
	}
}


void AddCNFConstraint(int NumTimeFrame)
{
	KaiGATE *tmpptr;
	string inname;
	KaiGATE *inptr;
	string cnfname;
	char tmp[100];

	for(unsigned int IdxCNF=0; IdxCNF<CNFCONSTRAINT.NoCNF(); ++IdxCNF) {
		CNFGate *cnf=CNFCONSTRAINT.GetCNF(IdxCNF);
		if(cnf->Is_Global()) {
			for(int IdxTF=0; IdxTF<NumTimeFrame; ++IdxTF) {
				sprintf(tmp, "%d", IdxTF);
				KaiGATE *gptr=new KaiGATE;
				cnfname=cnf->GetName()+"_t"+tmp;
				if(!gptr) { cerr<<"No Memory !"<<endl; exit(-1); }
				if(CUTNameTable.find(cnfname)!=CUTNameTable.end()) {
					cerr<<"Duplicated Name in CNF: "<<cnfname<<endl; exit(-1);
				}
				gptr->SetName(cnfname);
				gptr->SetFunction(G_CNF);
				for(unsigned int i=0; i<cnf->NoFanin(); ++i) {
					tmpptr=KaiNameTable[cnf->GetFanin(i)];
					if(!tmpptr) { cerr<<"Missing ... "<<endl; exit(-1); }
					if(tmpptr->GetFunction()==G_DFF) {
						inname=cnf->GetFanin(i)+"_ppi_t"+tmp;
						inptr=CUTNameTable[inname];
						if(!inptr) { cerr<<"Missing CUT: "<<inname<<endl; exit(-1); }
						gptr->AddFanin(inptr, cnf->GetSign(i));
						inptr->AddFanout(gptr);
					}
					else {
						inname=cnf->GetFanin(i)+"_dup_t"+tmp;
						inptr=CUTNameTable[inname];
						if(!inptr) { cerr<<"Missing CUT: "<<inname<<endl; exit(-1); }
						gptr->AddFanin(inptr, cnf->GetSign(i));
						inptr->AddFanout(gptr);
					}
				}
				CUT.AddGate(gptr);
				CUT.AddCNF(gptr);
			}
		}
		else {
			KaiGATE *gptr=new KaiGATE;
			cnfname=cnf->GetName();
			if(!gptr) { cerr<<"No Memory !"<<endl; exit(-1); }
			if(CUTNameTable.find(cnfname)!=CUTNameTable.end()) {
				cerr<<"Duplicated Name in CNF: "<<cnfname<<endl; exit(-1);
			}
			gptr->SetName(cnfname);
			gptr->SetFunction(G_CNF);
			for(unsigned int i=0; i<cnf->NoFanin(); ++i) {
				if(atoi(cnf->GetTimeFrame(i).c_str())>=NumTimeFrame) {
					cerr<<"Too Much TimeFrame Specified in CNF File"<<endl; exit(-1);
				}
				tmpptr=KaiNameTable[cnf->GetFanin(i)];
				if(!tmpptr) { cerr<<"Missing ... "<<endl; exit(-1); }
				if(tmpptr->GetFunction()==G_DFF) {
					inname=cnf->GetFanin(i)+"_ppi_t"+cnf->GetTimeFrame(i);
					inptr=CUTNameTable[inname];
					if(!inptr) { cerr<<"Missing CUT: "<<inname<<endl; exit(-1); }
					gptr->AddFanin(inptr, cnf->GetSign(i));
					inptr->AddFanout(gptr);
				}
				else {
					inname=cnf->GetFanin(i)+"_dup_t"+cnf->GetTimeFrame(i);
					inptr=CUTNameTable[inname];
					if(!inptr) { cerr<<"Missing CUT: "<<inname<<endl; exit(-1); }
					gptr->AddFanin(inptr, cnf->GetSign(i));
					inptr->AddFanout(gptr);
				}
			}
			CUT.AddGate(gptr);
			CUT.AddCNF(gptr);
		}
	}
}
