
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

#ifndef KAIPATTERN_H
#define KAIPATTERN_H

#include <string>
#include <vector>
#include <iostream>
#include "kai_typeemu.h"

class KaiPattern
{
 public:
	 KaiPattern() {
		 _pi_value.clear();
		 _scan_value.clear();
	 }
	 ~KaiPattern() {};
	 unsigned int NoTimeFrame() { return _pi_value.size(); }
	 vector<vector<int> >& PiValue() { return _pi_value; }
	 vector<int>& ScanValue() { return _scan_value; }
	 void Dump();
	 
 private:
	 vector<vector<int> > _pi_value;
	 vector<int> _scan_value;
};


class PatternList
{
 public:
	 PatternList() { _success.clear();
	 }
	 ~PatternList() {};
	 unsigned int NoPttn() { return _pttn.size(); }
	 KaiPattern* GetPttn(unsigned int i) { return _pttn[i]; }
	 void AddPttn(KaiPattern *p) { _pttn.push_back(p); }
	 void Dump();
	 void WriteOut(int);
	 void RecordFail(){_success.push_back(0);}
	 void RecordSuccess(){_success.push_back(1);}
	 
 private:
	 vector<KaiPattern*> _pttn;
	 vector<int> _success;
};

#endif
