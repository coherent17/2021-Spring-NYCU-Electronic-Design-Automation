
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
#include <string>
#include <vector>
#include "GetLongOpt.h"
#include "kai_netlist.h"

void KaiNetlist::Dump()
{
	cout<<"------------- PI -------------"<<endl;
	for(unsigned int i=0; i<_pi.size(); ++i) {
		KaiGATE *g=_pi[i];
		cout<<g->GetName()<<" --> in: ";
		for(unsigned int j=0; j<g->NoFanin(); ++j)
			cout<<g->Fanin(j)->GetName()<<" ";
		cout<<" out: ";
		for(unsigned int j=0; j<g->NoFanout(); ++j)
			cout<<g->Fanout(j)->GetName()<<" ";
		cout<<endl;
	}
	cout<<"------------- PO -------------"<<endl;
	for(unsigned int i=0; i<_po.size(); ++i) {
		KaiGATE *g=_po[i];
		cout<<g->GetName()<<" --> in: ";
		for(unsigned int j=0; j<g->NoFanin(); ++j)
			cout<<g->Fanin(j)->GetName()<<" ";
		cout<<" out: ";
		for(unsigned int j=0; j<g->NoFanout(); ++j)
			cout<<g->Fanout(j)->GetName()<<" ";
		cout<<endl;
	}
	cout<<"------------- GATE -------------"<<endl;
	for(unsigned int i=0; i<_netlist.size(); ++i) {
		KaiGATE *g=_netlist[i];
		cout<<g->GetName()<<" --> in: ";
		for(unsigned int j=0; j<g->NoFanin(); ++j)
			cout<<g->Fanin(j)->GetName()<<" ";
		cout<<" out: ";
		for(unsigned int j=0; j<g->NoFanout(); ++j)
			cout<<g->Fanout(j)->GetName()<<" ";
		cout<<endl;
	}
	cout<<"------------- DFF -------------"<<endl;
	for(unsigned int i=0; i<_dff.size(); ++i) {
		KaiGATE *g=_dff[i];
		cout<<g->GetName()<<" --> in: ";
		for(unsigned int j=0; j<g->NoFanin(); ++j)
			cout<<g->Fanin(j)->GetName()<<" ";
		cout<<" out: ";
		for(unsigned int j=0; j<g->NoFanout(); ++j)
			cout<<g->Fanout(j)->GetName()<<" ";
		cout<<endl;
	}
	cout<<"------------- CNF -------------"<<endl;
	for(unsigned int i=0; i<_cnf.size(); ++i) {
		KaiGATE *g=_cnf[i];
		cout<<g->GetName()<<" --> in: ";
		for(unsigned int j=0; j<g->NoFanin(); ++j)
			cout<<g->Fanin(j)->GetName()<<" ";
		cout<<" out: ";
		for(unsigned int j=0; j<g->NoFanout(); ++j)
			cout<<g->Fanout(j)->GetName()<<" ";
		cout<<endl;
	}
}
