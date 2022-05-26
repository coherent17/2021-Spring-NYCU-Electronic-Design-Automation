
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

#ifndef KAIOBJECTIVE_H
#define KAIOBJECTIVE_H
#include <string>
#include <vector>
#include "kai_path.h"


class AtpgObj
{
 public:
	 AtpgObj() {}
	 ~AtpgObj() {}
	 void AddObj(string s, unsigned int v) { _name.push_back(s); _value.push_back(v); }
	 unsigned int NoObj() { return _name.size(); }
	 string GetObjName(unsigned int i) { return _name[i]; }
	 unsigned int GetObjValue(unsigned int i) { return _value[i]; }
	 void Dump();
	 void BuildFromPath_NR(PATH*);
	 void BuildFromPath_R(PATH*);
	 void cleanup() { _name.clear(); _value.clear(); }
	 
 private:	 
	 vector<string> _name;
	 vector<unsigned int> _value;
};

#endif
