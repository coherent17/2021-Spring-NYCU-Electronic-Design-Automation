
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

#ifndef CSAT_MEMORY_H
#define CSAT_MEMORY_H
using namespace std;
#include <vector>
#include "csat_macro.h"
#include <cstddef>

struct SFree
{
SFree* _next;
};

struct SMemoryManager
{
vector<char*> _vblock;
int _block_size;
SFree* _freelist;

//constructs & destructs
SMemoryManager(int block_size)
    {
    _freelist = NULL;
    _block_size = block_size;
    }
SMemoryManager()
    {
    _freelist = NULL;
    _block_size = 10;
    }
~SMemoryManager()
    {
    free();
    }
void free()
    {
    for (int i = 0; i < _vblock.size();i++)
	{
	delete [] _vblock[i];
	}
    _vblock.clear();
    _freelist = NULL;
    }
};

struct SMemoryGlobal
{
SMemoryManager _target;
SMemoryManager _unique_clause;
SMemoryManager _two_literal_clause;
SMemoryManager _watched_clause;
SMemoryManager _resolution;
SMemoryManager _three_literal_clause;
};

extern SMemoryGlobal gmng;

#endif
