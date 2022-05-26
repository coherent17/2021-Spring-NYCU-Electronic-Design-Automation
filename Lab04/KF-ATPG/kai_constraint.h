
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

#ifndef KAICONSTRAINT_H
#define KAICONSTRAINT_H

#include <string>
#include <vector>

/* [CONSTRAINT FILE FORMAT]
   
	# comment
	[CNF gate name] [timeframe] [signed signal] [timeframe] [signal]
	# example
	C * -A * -B  ("A" and "B" can't be 1 at all timeframe "*")
	C 1 +A 2 +B 

*/

class CNFGate
{
 public:
	 CNFGate() { 
		 _fanin.clear();
		 _sign.clear();
		 _timeframe.clear();
	 }
	 ~CNFGate() {}
	 void SetName(string n) { _name=n; }
	 string GetName() { return _name; }
	 void AddFanin(string s) { _fanin.push_back(s); }
	 string GetFanin(unsigned int i) { return _fanin[i]; }
	 void AddSign(bool b) { _sign.push_back(b); }
	 bool GetSign(unsigned int i) { return _sign[i]; }
	 void AddTimeFrame(string t) { _timeframe.push_back(t); }
	 string GetTimeFrame(unsigned int i) { return _timeframe[i]; }
	 unsigned int NoFanin() { return _fanin.size(); }
	 bool Is_Global() { return _global; }
	 void SetGlobal(bool b) { _global=b; }
	 
 private:
	 string _name;
	 vector<string> _fanin;
	 vector<bool> _sign;
	 vector<string> _timeframe;
	 bool _global;
};


class KaiConstraint
{
 public:
	 KaiConstraint() { _clist.clear(); };
	 ~KaiConstraint() {};
	 void ReadIn();
	 void Dump();
	 CNFGate* GetCNF(unsigned int i) { return _clist[i]; }
	 unsigned int NoCNF() { return _clist.size(); }

 private:	 
	 void Check();
	 vector<CNFGate*> _clist;
};

#endif
