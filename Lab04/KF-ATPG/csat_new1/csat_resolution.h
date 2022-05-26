
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

#ifndef CSAT_RESOLUTION_H
#define CSAT_RESOLUTION_H

#include "csat_gate.h"

#define  MAX_RESOLUTION_NUM 4 
struct S_Resolution
{

int _num;
int _literals[MAX_RESOLUTION_NUM];
int _level;
S_Resolution* _parents[2];
void* operator new(size_t size);
void operator delete(void* ptr);
void trace_root();
bool is_conflict(CCircuit& circuit);
bool is_sat(CCircuit& circuit);
int find_max_conflict_dlevel(CCircuit& circuit);
int find_unit_signal(CCircuit& circuit);

S_Resolution()
    {
    _parents[0] = NULL;
    _parents[1] = NULL;
    _level = 0;
    }
};
NEW(S_Resolution, gmng._resolution);
DELETE(S_Resolution, gmng._resolution);


struct S_resolutionhash
{
int operator() (S_Resolution* reso, const int size)
    {
    int result = 0;
    int tmpand = ~0;
    int tmpor = 0;
    result += reso->_num;
    for (int i = 0; i < reso->_num;i++)
        {
        tmpand = tmpand & reso->_literals[i];
        tmpor = tmpor | reso->_literals[i];
        result += reso->_literals[i];
        }
    result += tmpand + tmpor;
    return abs(result%size);
    }
};

struct S_resolutioncpr
{
bool equal(S_Resolution* r1, S_Resolution* r2)
    {
    if (r1->_num != r2->_num)
        return false;
    for (int i = 0; i < r1->_num;i++)
        {
        if (r1->_literals[i] != r2->_literals[i])
            return false;
        }
    return true;
    }
};

#endif
