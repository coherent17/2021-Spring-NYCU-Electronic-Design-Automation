
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
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include "GetLongOpt.h"
#include "kai_netlist.h"

extern char* kktext;
extern FILE *kkin;
extern int kkparse(void);
extern bool ParseError;

extern GetLongOpt option;
extern KaiNetlist NETLIST;
extern map<string, KaiGATE*> KaiNameTable;

void KaiNetlist::FanoutList()
{
    KaiGATE* gptr;
    for(unsigned int i=0; i<NoGate(); ++i) {
        gptr=Gate(i);
        for(unsigned int j=0; j<gptr->NoFanin(); ++j) {
            gptr->Fanin(j)->AddFanout(gptr);
        }
    }
}


void KaiNetlist::InitStructure() 
{
	for(unsigned int i=0; i<_netlist.size(); ++i) {
    	KaiGATE* gptr=_netlist[i];
        switch(gptr->GetFunction()) {
        case G_PI: _pi.push_back(gptr);
            break;
        case G_PO: _po.push_back(gptr);
            break;
		case G_DFF: _dff.push_back(gptr);
		 	break;
		case G_XOR: _xor.push_back(gptr);
			break;			
		case G_CNF: _cnf.push_back(gptr);
			break;
        default:
            break;
        }
	}
}


void KaiNetlist::ReadIn()
{
    KaiGATE* gptr;
	// Create VDD Gate
	gptr=new KaiGATE;
	if(!gptr) { cerr<<"No Memory!"<<endl; exit(-1); }
	gptr->SetName("vdd");
	gptr->SetFunction(G_BUF);
	gptr->SetVDD(true);
	gptr->SetInverse(false);
	KaiNameTable["vdd"]=gptr;
	NETLIST.AddGate(gptr);
	// Create GND Gate
	gptr=new KaiGATE;
	if(!gptr) { cerr<<"No Memory!"<<endl; exit(-1); }
	gptr->SetName("gnd");
	gptr->SetFunction(G_BUF);
	gptr->SetGND(true);
	gptr->SetInverse(false);
	KaiNameTable["gnd"]=gptr;
	NETLIST.AddGate(gptr);
	
    if(option.retrieve("circuit")) {
        if((kkin=fopen(option.retrieve("circuit"),"r"))==NULL) {
            cout<<"Can't open circuit file:"<<option.retrieve("circuit")<<endl;
            exit(-1);
        }
    }
    else { cout<<"Input circuit file missing"<<endl; option.usage(); exit(-1); }
    cout<<"Start parsing input file: "<<option.retrieve("circuit")<<endl;
    kkparse();
    if(ParseError) {
        cerr<<"Please Correct Parsing Error and Try Again"<<endl;
		exit(-1);
    }
    fclose(kkin);
    FanoutList();
    InitStructure();
	//Dump();
}


// --------------------------------------------------- //
// ReOrder the Scan-Chain Ordering if Necessary
// --------------------------------------------------- //
void KaiNetlist::ArrangeScan()
{
	_ScanChain.clear();
	if(option.retrieve("chain")) {
		cout<<"Re-Order the Scan-Chain ... "<<endl;
		ifstream CHAINFILE(option.retrieve("chain"));
		if(!CHAINFILE) { cerr<<"Can't Open file: "<<option.retrieve("chain")<<endl; exit(-1); }
		string inputString;
		vector<string> tmp;
		string str;
		unsigned int ScanIdx=0;
		while(getline(CHAINFILE, inputString)!=NULL) {
			istringstream iss(inputString);
			tmp.clear();
			while(getline(iss, str, ' '))
				tmp.push_back(str);
			if(tmp.size()!=1) { cerr<<"Scan Chain File Format Error"<<endl; exit(-1); }
			if(_ScanChain.find(tmp[0])!=_ScanChain.end()) {
				cerr<<"Duplicated Scan Chain Specification !"<<endl; exit(-1);
			}
			_ScanChain[tmp[0]]=ScanIdx;
			ScanIdx++;
		}
		CHAINFILE.close();
		vector<KaiGATE*> tmpdff;
		tmpdff.resize(_dff.size());
		for(unsigned int i=0; i<_dff.size(); ++i) {
			if(_ScanChain.find(_dff[i]->GetName())==_ScanChain.end()) {
				cerr<<"Wrong Scan Name !"<<endl; exit(-1);
			}
			tmpdff[_ScanChain[_dff[i]->GetName()]]=_dff[i];
		}
		_dff.clear();
		_dff=tmpdff;
		//Dump();
	}
	else {
		for(unsigned int i=0; i<_dff.size(); ++i) {
			if(_ScanChain.find(_dff[i]->GetName())!=_ScanChain.end()) {
				cerr<<"No La !"<<endl; exit(-1);
			}
			_ScanChain[_dff[i]->GetName()]=i;
		}
	}
}


unsigned int KaiNetlist::ScanID(string s)
{
	if(_ScanChain.find(s)==_ScanChain.end()) {
		cerr<<"No DFF Name as = "<<s<<endl; exit(-1);
	}
	return _ScanChain[s];
}
