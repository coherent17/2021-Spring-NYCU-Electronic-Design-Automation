#pragma GCC diagnostic ignored "-Wwrite-strings"
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
#include <string>
#include <vector>
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

double get_cpu_time();
double show_time(double, string);
int SetupOption(int, char**);
void Version();
int ExpandCircuit();
void ConvertToSNetlist();
void RecordPattern(CSAT_Manager, int);

GetLongOpt option;
KaiNetlist NETLIST;
map<string, KaiGATE*> KaiNameTable;
KaiNetlist CUT;
KaiConstraint CNFCONSTRAINT;
SNetlist LuCUT;
PatternList PTTN;

int main(int argc, char **argv)
{
	Version();
    double time_begin, time_tmp;
    time_begin=get_cpu_time();
	time_tmp=time_begin;
    int optind=SetupOption(argc,argv);

	NETLIST.ReadIn();
	NETLIST.ArrangeScan();
	if(option.retrieve("constraint"))
		CNFCONSTRAINT.ReadIn();

	PATH P;
	P.Initialize();
	
	int NumTimeFrame=ExpandCircuit();
	
	ConvertToSNetlist();
	time_tmp=show_time(time_tmp, "Construct Circuit");

	time_tmp=show_time(time_tmp, "Build ATPG Objectives");

	CSAT_Manager solver=CSAT_InitManager();
	CSAT_Netlist(solver, &LuCUT);
	CSAT_SolveInit(solver);
	CSAT_SetSolveOption(solver, "debug level", "0");
	char **tname;
	int *tvalue;
	
	AtpgObj OBJ;

	
	int NumTestable=0;
	int NumUnTestable=0;
	int NumOthers=0;
	
	while(!P.eof()){
		P.ReadNextPath();
		if(!option.retrieve("atpg")) OBJ.BuildFromPath_NR(&P);
		else if(strcmp(option.retrieve("atpg"), "NR")==0) OBJ.BuildFromPath_NR(&P);
		else if(strcmp(option.retrieve("atpg"), "R")==0) OBJ.BuildFromPath_R(&P);
		else { cerr<<"Please Set \"-atpg\" as NR or R"<<endl; exit(-1); }		

		tname=(char**) malloc(sizeof(char*)*OBJ.NoObj());
		tvalue=(int*) malloc(sizeof(int)*OBJ.NoObj());
		for(unsigned int i=0; i<OBJ.NoObj(); ++i) {
			tname[i]=strdup((char*)OBJ.GetObjName(i).c_str());
			tvalue[i]=OBJ.GetObjValue(i);
		}
		CSAT_AddTarget(solver, OBJ.NoObj(), tname, tvalue);
		CSAT_Solve(solver);
		if(CSAT_Get_Target_Result(solver, 0)==1){
			P.SetStatus(UNTESTABLE);
			PTTN.RecordFail();
		}
		else if(CSAT_Get_Target_Result(solver, 0)==2) { 
			P.SetStatus(TESTABLE); 
			PTTN.RecordSuccess();
			RecordPattern(solver, NumTimeFrame);
		}
		else { P.SetStatus(OTHERS); }
		for(unsigned int i=0; i<OBJ.NoObj(); ++i)
			free(tname[i]);
		free(tname);
		free(tvalue);
		CSAT_ClearTargets(solver);
		//result
		if(P.GetStatus()==TESTABLE) { 
			NumTestable++;
		}
		else if(P.GetStatus()==UNTESTABLE) {
			//cout<<" UNTESTABLE ID :"<<IdxP<<endl;
			NumUnTestable++;
		}
		else if(P.GetStatus()==OTHERS) { 
			NumOthers++;
		}
		else { cerr<<"NO TRY YOUR BIG HEAD !"<<endl; exit(-1); }
	}
	cout<<endl;
	//PTTN.Dump();
	time_tmp=show_time(time_tmp, "Solve ATPG Objectives");
	if(option.retrieve("output")) PTTN.WriteOut(NumTimeFrame);
	

	cout<<"// ----------------------------------"<<endl;
	if(!option.retrieve("atpg")) cout<<"// NON-ROBUST";
	else if(strcmp(option.retrieve("atpg"), "SR")==0) cout<<"// SEMI-ROBUST";
	else cout<<"// NON-ROBUST";
	cout<<" Path Sensitization Criteria"<<endl;

	cout<<"// Scan Chain: ";
	cout<<"Functional-Justification"<<endl;
	cout<<"// Number of Time Frame = "<<NumTimeFrame<<endl;

	if(option.retrieve("output")) {
		cout<<"// Set Unknown PI to ";
		if(!option.retrieve("unknown")) cout<<"ZERO"<<endl;
		else if(strcmp(option.retrieve("unknown"), "ZERO")==0) cout<<"ZERO"<<endl;
		else if(strcmp(option.retrieve("unknown"), "ONE")==0) cout<<"ONE"<<endl;
		else if(strcmp(option.retrieve("unknown"), "RAND")==0) cout<<"RANDOM"<<endl;
		else if(strcmp(option.retrieve("unknown"), "LESS")==0) cout<<"LESS"<<endl;
		else cout<<"X"<<endl;
	}

	cout<<"// Number of Testable Path = "<<NumTestable<<endl;
	cout<<"// Number of Un-Testable Path = "<<NumUnTestable<<endl;
	cout<<"// Number of NOT SOLVED Path = "<<NumOthers<<endl;
	cout<<"// Test Coverage = "<<((double)NumTestable/(double)P.NoPath())*100<<"%"<<endl;
	cout<<"// ----------------------------------"<<endl;
	time_tmp=show_time(time_begin, "Total Process");
}
