
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

#ifndef KAIPATH_H
#define KAIPATH_H

#include <vector>
#include "kai_gate.h"
#include <fstream>
enum TRANSITION { R=0, F };
enum PATH_STATUS { NOTRY=0, TESTABLE, UNTESTABLE, OTHERS };

class PATH
{
 public:
    PATH() 
	{
		no_path=0;
		tran=true;
		_vgate.clear();
		_vtrans.clear();
		_status=NOTRY;
	};
    ~PATH() {};
    PATH_STATUS GetStatus() { return _status; }
	void SetStatus(PATH_STATUS s) { _status=s; }
	void AddGate(KaiGATE *g) { _vgate.push_back(g); }
	void AddTrans(TRANSITION t) { _vtrans.push_back(t); }
	KaiGATE* GetGate(unsigned int i) { return _vgate[i]; }
	TRANSITION GetTrans(unsigned int i) { return _vtrans[i]; }
	unsigned int NoGate() { return _vgate.size(); }
	unsigned int NoTrans() { return _vtrans.size(); }
	void Dump();
	
	void Initialize();
	bool eof()
	{
		return (pathinput.eof());
	}
	void ReadNextPath();
	void cleanup()
	{
		_vgate.clear();
		_vtrans.clear();
		_status=NOTRY;
	};
	int NoPath() { return no_path; }
	
 private:
    vector<KaiGATE*> _vgate;
    vector<TRANSITION> _vtrans;
	PATH_STATUS _status;
	
	ifstream pathinput;
	int no_path;
	bool tran;
};


#endif
