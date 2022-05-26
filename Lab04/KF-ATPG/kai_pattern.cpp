
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
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include "GetLongOpt.h"
#include "kai_gate.h"
#include "kai_netlist.h"
#include "kai_path.h"
#include "kai_constraint.h"
#include "kai_objective.h"
#include "kai_pattern.h"
// Lu's Solver
#include "csat_new1/csat_netlist.h"
#include "csat_new1/csat.h"

extern GetLongOpt option;
extern KaiNetlist NETLIST;
extern KaiNetlist CUT;
extern SNetlist LuCUT;
extern PatternList PTTN;

int NameToValue(CSAT_Manager solver, string name)
{
	int ID=CSAT_Name_To_IDS(solver, name);
	return CSAT_Signal_Value(solver, ID);
}

void RecordPattern(CSAT_Manager solver, int NumTimeFrame)
{
	char tmp[100];
	vector<int> pivalue;

	KaiPattern *pttnptr=new KaiPattern;
	if(!pttnptr) { cerr<<"No Memory !"<<endl; exit(-1); }

	for(unsigned int IdxDFF=0; IdxDFF<NETLIST.NoDFF(); ++IdxDFF) {
		string dffname="PPI_"+NETLIST.DFFGate(IdxDFF)->GetName();
		if(LuCUT._name_table(dffname)==NULL) {
			cerr<<"Missing PPI"<<endl; exit(-1);
		}
		pttnptr->ScanValue().push_back(NameToValue(solver, dffname));
	}
	for(unsigned int IdxTF=0; IdxTF<NumTimeFrame; ++IdxTF) {
		pivalue.clear();
		sprintf(tmp, "%d", IdxTF);
		for(unsigned int IdxPI=0; IdxPI<NETLIST.NoPI(); ++IdxPI) {
			string piname=NETLIST.PIGate(IdxPI)->GetName()+"_dup_t"+tmp;
			if(LuCUT._name_table(piname)==NULL) {
				cerr<<"Missing PI"<<endl; exit(-1);
			}
			pivalue.push_back(NameToValue(solver, piname));
		}
		pttnptr->PiValue().push_back(pivalue);
	}
	PTTN.AddPttn(pttnptr);
}


void KaiPattern::Dump()
{
	for(unsigned int i=0; i<_pi_value.size(); ++i) {
		cout<<"PI-"<<i<<": ";
		for(unsigned int j=0; j<_pi_value[i].size(); ++j) {
			cout<<_pi_value[i][j];
		}
		cout<<endl;
	}
	cout<<"SCAN: ";
	for(unsigned int i=0; i<_scan_value.size(); ++i) {
		cout<<_scan_value[i];
	}
	cout<<endl;
}


void PatternList::Dump()
{
	cout<<" ------------------- DUMP PATTERN ------------------- "<<endl;
	for(unsigned int i=0; i<_pttn.size(); ++i)
		_pttn[i]->Dump();
	cout<<" ------------------- DUMP PATTERN ------------------- "<<endl;
}


void PatternList::WriteOut(int NumTimeFrame)
{
	srand(time(NULL));
	int no_fail=0;
	enum ATPGTYPE { ZERO=0, ONE, RAND, X , LESS};
	ATPGTYPE type;
	if(!option.retrieve("unknown")) type=X;
	else if(strcmp(option.retrieve("unknown"), "ZERO")==0) type=ZERO;
	else if(strcmp(option.retrieve("unknown"), "ONE")==0) type=ONE;
	else if(strcmp(option.retrieve("unknown"), "RAND")==0) type=RAND;
	else if(strcmp(option.retrieve("unknown"), "LESS")==0) type=LESS;
	else type=X;

    ofstream PF(option.retrieve("output"));
    if(!PF) { cerr<<"Can't Open file: "<<option.retrieve("output")<<endl; exit(-1); }
	//PF<<"# Number of Time Frame = "<<NumTimeFrame<<endl;
	for(unsigned int IdxPI=0; IdxPI<NETLIST.NoPI(); ++IdxPI)
		PF<<"PI "<<NETLIST.PIGate(IdxPI)->GetName()<<endl;
	for(unsigned int IdxFF=0; IdxFF<NETLIST.NoDFF(); ++IdxFF)
		PF<<"FF "<<NETLIST.DFFGate(IdxFF)->GetName()<<endl;
	
	for(int j=0; j<_success.size(); ++j) {		
		if(_success[j]==0) {
			no_fail++;
			PF<<"Untestable path"<<endl;
			continue;
		}
		KaiPattern *p=_pttn[j-no_fail];
		for(unsigned int IdxTF=0; IdxTF<p->PiValue().size(); ++IdxTF) {
			for(unsigned int i=0; i<p->PiValue()[IdxTF].size(); ++i) {
				if(p->PiValue()[IdxTF][i]==1||p->PiValue()[IdxTF][i]==0) PF<<p->PiValue()[IdxTF][i];
				else {
					assert(p->PiValue()[IdxTF][i]==2);
					if(type==ZERO) PF<<"0";
					else if(type==ONE) PF<<"1";
					else if(type==RAND) PF<<rand()%2;
					else if(type==LESS) {
						if(p->PiValue().size()!=2) {
							cerr<<"LESS needs to have TimeFrame=2 !"<<endl;
							exit(-1);
						}
						if(IdxTF==0) {
							if(p->PiValue()[IdxTF+1][i]==2) {
								int tmp=rand()%2;
								PF<<tmp;
								p->PiValue()[IdxTF][i]=tmp;
							}
							else
								PF<<p->PiValue()[IdxTF+1][i];
						}
						else if(IdxTF==1) {
							if(p->PiValue()[IdxTF-1][i]==2) {
								cerr<<"Something WRONG at LESS !"<<endl;
								exit(-1);
							}
							else
								PF<<p->PiValue()[IdxTF-1][i];
						}
						else { cerr<<"Impossible !"<<endl; exit(-1); }
					}
					else PF<<"X";
				}
			}
			PF<<endl;
		}
		for(unsigned int i=0; i<p->ScanValue().size(); ++i) {
			if(p->ScanValue()[i]==1||p->ScanValue()[i]==0) PF<<p->ScanValue()[i];
			else {
				assert(p->ScanValue()[i]==2);
				if(type==ZERO) PF<<"0";
				else if(type==ONE) PF<<"1";
				else if(type==RAND) PF<<rand()%2;
				else if(type==LESS) {
					cerr<<"LESS Not Implemented Yet Here  !"<<endl;
					exit(-1);
				}
				else PF<<"X";
			}
		}
		if(p->ScanValue().size()!=0)
			PF<<endl;
	}
	PF.close();
}
