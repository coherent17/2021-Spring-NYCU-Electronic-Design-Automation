
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

#ifndef KAIGATE_H
#define KAIGATE_H

#include <string>
#include <vector>
#include <iostream>
#include "kai_typeemu.h"

class KaiGATE 
{
 public:
    KaiGATE()
	{
		_function=G_BAD;
        _fanin.clear();
        _fanout.clear();
		_fanin_sign.clear();
		_vdd=false;
		_gnd=false;
    }
	void SetName(string n) { _name=n; }
	string GetName() { return _name; }
	void SetFunction(GATEFUNC f) { _function=f; }
	GATEFUNC GetFunction() { return _function; }
	KaiGATE* Fanin(unsigned int i) { return _fanin[i]; }
	bool FaninSign(unsigned int i) { return _fanin_sign[i]; }
	KaiGATE* Fanout(unsigned int i) { return _fanout[i]; }
	void AddFanin(KaiGATE *g, bool s=false) { _fanin.push_back(g); _fanin_sign.push_back(s); }
	void AddFanout(KaiGATE *g) { _fanout.push_back(g); }
	unsigned int NoFanin() { return _fanin.size(); }
	unsigned int NoFanout() { return _fanout.size(); }
	void SetInverse(bool i) { _inverse=i; }
	bool Is_Inverse() { return _inverse; }
	unsigned int NoFaninSign() { return _fanin_sign.size(); }
	bool GetVDD() { return _vdd; }
	void SetVDD(bool b) { _vdd=b; }
	bool GetGND() { return _gnd; }
	void SetGND(bool b) { _gnd=b; }

 private:
    string _name;
	GATEFUNC _function;
    vector<KaiGATE*> _fanin;
    vector<KaiGATE*> _fanout;
	vector<bool> _fanin_sign;
	bool _inverse;
	bool _vdd;
	bool _gnd;
};

#endif
