
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
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "GetLongOpt.h"
#include "kai_gate.h"
#include "kai_netlist.h"
#include "kai_path.h"
#include <cstdlib>

extern GetLongOpt option;
extern KaiNetlist NETLIST;
extern map<string, KaiGATE*> KaiNameTable;

void PATH::Initialize() {
	if(option.retrieve("path_t")&&option.retrieve("path_not")) {
		cerr<<"Please Specify Only One \"-path_t\" or \"-path_not\" !"<<endl; exit(-1);
	}
	if(option.retrieve("path_t")) {
		pathinput.open(option.retrieve("path_t"), ios::in);
		if (!pathinput) {
			cout << "Unable to open input path file: " << option.retrieve("path_t") << endl;
			exit( -1);
		}
		tran=true;
	}
	else if(option.retrieve("path_not")) {
		pathinput.open(option.retrieve("path_not"), ios::in);
		if (!pathinput) {
			cout << "Unable to open input path file: " << option.retrieve("path_not") << endl;
			exit( -1);
		}
		tran=false;
	}
	else { cerr<<"Please Specify Path File!"<<endl; exit(-1); }
		
	string instring;
	pathinput >> instring; //@
	while(instring!="@") pathinput >> instring; //@
} 

void PATH::ReadNextPath()
{
	KaiGATE* gptr;
	string instring;
	string t;
	cleanup();
	//cout<<"read path "<<(no_path+1)<<endl;
    if(tran){ //transition		
		pathinput >> instring; //ex. Gi, G2, G3...
		do{
			gptr=KaiNameTable[instring];	
			if(gptr==NULL) { cerr<<"Can't Find GATE from Path !"<<endl; exit(-1); }	
			AddGate(gptr);			
			pathinput >> instring; //R or F TRAN_STATUS { RISING, FALLING

			if(instring=="R") AddTrans(R);
			else if(instring=="F") AddTrans(F);
			else { cerr<<"Invalid Transition Type !"<<endl; exit(-1); }
			/*
			cout<< gptr->GetName()<<" ";
			if(gptr->GetTranStatus()==RISING) cout<< "R"<<" ";
			else cout<< "F"<<" ";
			*/
			
			pathinput >> instring; //@
			if(pathinput.eof()) break;
		}while(instring!="@");
		no_path++;
		SetStatus(NOTRY);
		//cout<<endl;
	}
	else{ //path delay
		
		pathinput >> instring; //R or F TRAN_STATUS { RISING, FALLING
		t=instring;
		pathinput >> instring; //ex. Gi, G2, G3...
		
		do{
			gptr=KaiNameTable[instring];
			if(gptr==NULL) { cerr<<"Can't Find GATE from Path !"<<endl; exit(-1); }	
			AddGate(gptr);				
			//cout<< gptr->GetName()<<" ";
			if(gptr->GetFunction()==G_PI){
				if(t=="R") AddTrans(R);
				else if(t=="F") AddTrans(F);
				else { cerr<<"Invalid Transition Type !"<<endl; exit(-1); }
				
				//if(gptr->GetTranStatus()==RISING) cout<< "R"<<" ";
				//else cout<< "F"<<" ";
			}
			
			pathinput >> instring;
			if(pathinput.eof()) break;
			
		}while(instring!="@");
		no_path++;
		SetStatus(NOTRY);
		//cout<<endl;
		
		// Build Transition
		assert(NoTrans()==1);
		TRANSITION CurTrans=GetTrans(0);
		for(unsigned int IdxG=1; IdxG<NoGate(); ++IdxG) {
			KaiGATE *gptr=GetGate(IdxG);
			if(gptr->Is_Inverse()) {
				if(CurTrans==R) { 
					AddTrans(F);
					CurTrans=F;
				}
				else if(CurTrans==F) { 
					AddTrans(R);
					CurTrans=R;
				}
				else { cerr<<"R/F Error !"<<endl; exit(-1); }
			}
			else {
				AddTrans(CurTrans);
			}
		}
	}	
}

void PATH::Dump()
{
	assert(_vgate.size()==_vtrans.size());
	for(unsigned int i=0; i<_vgate.size(); ++i) {
		cout<<_vgate[i]->GetName()<<" ";
		if(_vtrans[i]==R) cout<<"R ";
		else if(_vtrans[i]==F) cout<<"F ";
		else { cerr<<"R/F Dump Error!"<<endl; exit(-1); }
	}
	cout<<endl;
}
