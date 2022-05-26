
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

#ifndef KAINETLIST_H
#define KAINETLIST_H

#include <string>
#include <vector>
#include <fstream>
#include <map>
#include "kai_gate.h"

class KaiNetlist
{
 public:
    KaiNetlist() 
	{
        _netlist.clear();
        _pi.clear();
        _po.clear();
		_dff.clear();
		_xor.clear();
		_cnf.clear();
    }

    void AddGate(KaiGATE* gptr) { _netlist.push_back(gptr); }
    void SetName(string n) { _name=n; }
    string GetName() { return _name; }

    KaiGATE* Gate(unsigned int index) { return _netlist[index]; }
    KaiGATE* PIGate(unsigned int index) { return _pi[index]; }
    KaiGATE* POGate(unsigned int index) { return _po[index]; }
	KaiGATE* DFFGate(unsigned int index) { return _dff[index]; }

    unsigned int NoGate() { return _netlist.size(); }
    unsigned int NoPI() { return _pi.size(); }
    unsigned int NoPO() { return _po.size(); }
	unsigned int NoDFF() { return _dff.size(); }
	unsigned int NoXOR() { return _xor.size(); }
	unsigned int NoCNF() { return _cnf.size(); }

	void AddCNF(KaiGATE *g) { _cnf.push_back(g); }

	void ReadIn();
	void ArrangeScan();
	void InitStructure();
	void Dump();
	unsigned int ScanID(string);

 private:
    void FanoutList();

    string _name;
    vector<KaiGATE*> _netlist;
    vector<KaiGATE*> _pi;
    vector<KaiGATE*> _po;
	vector<KaiGATE*> _dff;
	vector<KaiGATE*> _xor;
	vector<KaiGATE*> _cnf;
	map<string, unsigned int> _ScanChain;
};

#endif
