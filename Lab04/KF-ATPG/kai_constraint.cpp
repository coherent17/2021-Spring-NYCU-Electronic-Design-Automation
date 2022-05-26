
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
#include <sstream>
#include "GetLongOpt.h"
#include "kai_gate.h"
#include "kai_netlist.h"
#include "kai_constraint.h"
#include <cstdlib>

extern GetLongOpt option;
extern KaiNetlist NETLIST;
extern map<string, KaiGATE*> KaiNameTable;
extern map<string, KaiGATE*> CUTNameTable;
void Version();

void KaiConstraint::ReadIn()
{
	_clist.clear();
    ifstream CFILE(option.retrieve("constraint"));
    if(!CFILE) { cerr<<"Can't Open file: "<<option.retrieve("constraint")<<endl; exit(-1); }
    string inputString;
    cout<<"Reading Constraint File: "<<option.retrieve("constraint")<<endl;
    vector<string> tmp;
    string str;
    while(getline(CFILE, inputString)!=NULL) {
        istringstream iss(inputString);
        tmp.clear();
        while(getline(iss, str, ' '))
            tmp.push_back(str);
        if(tmp.empty()) continue;
		if(tmp[0][0]=='#') continue;
		if(tmp.size()%2==0) { cerr<<"Wrong CNF format !"<<endl; exit(-1); }

		CNFGate *cnf=new CNFGate;
		if(!cnf) { cerr<<"No Memory !"<<endl; exit(-1); }
		if(KaiNameTable.find(tmp[0])!=KaiNameTable.end()) {
			cerr<<"Duplicated CNF Gate Name: "<<tmp[0]<<endl; exit(-1);
		}
		cnf->SetName(tmp[0]);
		for(unsigned int i=1; i<tmp.size(); ++i) {
			if(i%2==1) {
				cnf->AddTimeFrame(tmp[i]);
			}
			else {
				if(tmp[i][0]=='-') cnf->AddSign(true);
				else if(tmp[i][0]=='+') cnf->AddSign(false);
				else { cerr<<"Error CNF Format: "<<tmp[i]<<endl; exit(-1); }
				string fg(tmp[i].begin()+1, tmp[i].end());
				if(KaiNameTable.find(fg)==KaiNameTable.end()) {
					cerr<<"Miss CNF Fanin: "<<fg<<endl; exit(-1);
				}
				cnf->AddFanin(fg);
			}
		}
		_clist.push_back(cnf);
    }
    CFILE.close();
	Check();
	cout<<"Number of Constraint CNF Gate = "<<_clist.size()<<endl;
	//Dump();
}


void KaiConstraint::Dump()
{
	cout<<" --------------- Dump CNF -------------- "<<endl;
	for(unsigned int i=0; i<_clist.size(); ++i) {
		CNFGate *g=_clist[i];
		cout<<g->GetName()<<" -> ";
		for(unsigned int j=0; j<g->NoFanin(); ++j)
			cout<<g->GetTimeFrame(j)<<" "<<g->GetSign(j)<<" "<<g->GetFanin(j)<<" ";
		cout<<endl;
	}
	cout<<" --------------- Dump CNF -------------- "<<endl;
}


void KaiConstraint::Check()
{
	cout<<"Checking CNF Format..."<<endl;
	for(unsigned int i=0; i<_clist.size(); ++i) {
		CNFGate *cnf=_clist[i];
		bool STARFLAG=false;
		bool NUMFLAG=false;
		for(unsigned int j=0; j<cnf->NoFanin(); ++j) {
			if(cnf->GetTimeFrame(j)=="*")
				STARFLAG=true;
			else
				NUMFLAG=true;
		}
		if(STARFLAG&&NUMFLAG) {
			Version();
			cerr<<"Current Version Only Support Either: "<<endl;
			cout<<" \"Global Constraint\" for All TimeFrames"<<endl;
			cout<<" \tor"<<endl;
			cout<<" \"Local Constraint across TimeFrames\""<<endl;
			exit(-1);
		}
		if(STARFLAG)
			cnf->SetGlobal(true);
		else
			cnf->SetGlobal(false);
	}
}
