
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

#ifndef CSAT_LITERAL_POOL_H
#define CSAT_LITERAL_POOL_H
using namespace std;
#include <assert.h>
#include "csat_clause.h"

#define START_SIZE 10000
#define HEAD_SIZE 1
#define GARBAGE_RATIO 1.5
#define GARBAGE_SIZE 100000
#define GROW_RATIO 2
#define DELETED 0

struct SLiteralPool
{
int* _head;
int* _tail;
int* _end;

//vector<CWatchedClause*> _vnclause;
vector<CUniqueClause*> _vunique;
//vector<CTwoLiteralClause*> _v2clause;
vector<CClause*> _vclause;
int _num_added_nclauses;
int _num_added_nliterals;
int _num_deleted_nclauses;
int _num_deleted_nliterals;
int _num_clause;
// construct & destruct functions
SLiteralPool()
    {
    _head = new int[START_SIZE];
    _tail = _head;
    _end = _head + START_SIZE;
    _num_added_nclauses = 0;
    _num_added_nliterals = 0;
    _num_deleted_nclauses = 0;
    _num_deleted_nliterals = 0;
    _num_clause = 0;
    }
~SLiteralPool()
    {
    //for (int i = 0; i < _vnclause.size();i++)
    //    {
    //    delete _vnclause[i];
    //    _vnclause[i] = NULL;
    //    }
    
    for (int i = 0; i < _vclause.size();i++)
        {
	if (_vclause[i]->type() == TWO_LITERAL_CLAUSE)
	    delete ((CTwoLiteralClause*)(_vclause[i]));
	else
            delete ((CWatchedClause*)(_vclause[i]));
        _vclause[i] = NULL;
        }
    for (int i = 0; i < _vunique.size();i++)
	{
	delete _vunique[i];
	_vunique[i] = NULL;
	}
    delete [] _head;
    _head = NULL;
    }
// data member asscess functioins
int* head() {return _head;}
int* end() {return _end;}
int* tail() {return _tail;}

// other functions
int size() {return _end - _head;}
int free_size() {return _end - _tail;}

CWatchedClause* get_clause_pointer(int* address) 
    {
    return (CWatchedClause*) (*address);
    }
void put_clause_pointer(CWatchedClause* ptr)
    {
    assert(sizeof(int) == sizeof(CWatchedClause*));
    *((CWatchedClause**)_tail) = ptr;
    _tail += HEAD_SIZE;
    assert((*(ptr->pliteral() -1)) == (int)ptr);
    }
void put_literal(int literal) {*_tail = literal; _tail++;}
int num_nclauses() {return _num_added_nclauses - _num_deleted_nclauses;}
int num_nliterals() {return _num_added_nliterals - _num_deleted_nliterals;} 

bool resize();
bool reuse();
void clear();
int cnf_size()
    {
    //return _vnclause.size()+_vunique.size()+_v2clause.size();
    return _vclause.size() + _vunique.size();
    }
};

#endif

