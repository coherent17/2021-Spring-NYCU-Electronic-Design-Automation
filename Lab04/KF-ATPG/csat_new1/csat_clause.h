
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

#ifndef CSAT_CLAUSE_H
#define CSAT_CLAUSE_H
using namespace std;
#include <vector>
#include <string>
#include <iostream>
#include "csat_macro.h"
#include "csat_const.h"
#include "csat_memory.h"
#include <stdlib.h>

// clause flag
#define ORIGINAL_CLAUSE 	0
#define CFTG_FOREVER 		1
#define CFTG_ASSUMED 		2
#define CFTG_UNREACH_STATE 	3
#define CFTG_LOOP_STATE 	4
#define CFTG_COMMON 		5
#define CFTG_DELETED 		6
#define RESOLUTION_CLAUSE	7

// clause type
#define UNIQUE_CLAUSE 		0
#define TWO_LITERAL_CLAUSE	1
#define WATCHED_CLAUSE		2
#define THREE_LITERAL_CLAUSE	3

class CCircuit;
class CCNFCircuit;

class CClause
{
protected:

char _type;
char _flag;
char _sel;
bool _in_use;
public:
// construct and destruct function
CClause() {_sel = 0;}
CClause(char ntype,char nflag)
    {
    _type = ntype;
    _flag = nflag;
    }
~CClause() {};

// data member access function

char& type() {return _type;}
char& flag() {return _flag;}
char& sel()  {return _sel;}
bool& in_use() {return _in_use;}

virtual int used_count() {return 0;}
virtual int num_literal() {cout<<"impossible "; return -1;}
virtual int& literal(int which) = 0;

virtual int find_frontier_variable(CCircuit& circuit, vector<int>& vfrontier) {return -1;}
virtual bool is_conflict(CCircuit& circuit) {return false;}
virtual int find_max_conflict_dlevel(CCircuit& circuit) {return 0;}
virtual int mark_signal (CCircuit& circuit, int id, int dlevel) {return 0;}
virtual int find_unit_signal(CCircuit& circuit) {return 0;}
virtual int unrelevance (CCircuit& circuit) {return 0;}
virtual int find_variable(CCircuit& circuit) {return -1;}
virtual int find_variable_all(CCircuit& circuit) {return -1;}
virtual int find_variable_old(CCircuit& circuit) {return -1;}
virtual void disconnect(CCircuit& circuit){};
virtual int find_variable_dec(CCircuit& circuit) {return -1;}


virtual bool is_conflict(CCNFCircuit& circuit) {return false;}
virtual int find_max_conflict_dlevel(CCNFCircuit& circuit) {return 0;}
virtual int mark_signal (CCNFCircuit& circuit, int id, int dlevel) {return 0;}
virtual int find_unit_signal(CCNFCircuit& circuit) {return 0;}
virtual int unrelevance (CCNFCircuit& circuit) {return 0;}
virtual int find_variable(CCNFCircuit& circuit) {return -1;}
virtual int find_variable_all(CCNFCircuit& circuit) {return -1;}
virtual int find_variable_old(CCNFCircuit& circuit) {return -1;}
virtual void disconnect(CCNFCircuit& circuit){};
virtual int find_variable_dec(CCNFCircuit& circuit) {return -1;}
virtual int find_variable_res(CCNFCircuit& circuit) {return -1;}
virtual CClause* optimize(CCNFCircuit& circuit) {return this;}
};

class CUniqueClause : public CClause
{
int _literal;

public:
// construct and destruct function
CUniqueClause()
    {
    type() = UNIQUE_CLAUSE; 
    }
CUniqueClause(char nflag)
    {
    _type = UNIQUE_CLAUSE;
    _flag = nflag;    
    
    }
~CUniqueClause() {};

// data member access functions
int& literal() {return _literal;}

// memory functions
void* operator new(size_t size);
void operator delete(void* ptr);

virtual int num_literal() {return 1;}
virtual int& literal(int which) {return _literal;}

virtual bool is_conflict(CCircuit& circuit);
virtual int find_unit_signal(CCircuit& circuit);
virtual int find_max_conflict_dlevel(CCircuit& circuit);
virtual int mark_signal (CCircuit& circuit, int id, int dlevel); 
virtual int find_variable_dec(CCircuit& circuit) {return _literal;}

virtual bool is_conflict(CCNFCircuit& circuit);
virtual int find_unit_signal(CCNFCircuit& circuit);
virtual int find_max_conflict_dlevel(CCNFCircuit& circuit);
virtual int mark_signal (CCNFCircuit& circuit, int id, int dlevel); 
virtual int find_variable_dec(CCNFCircuit& circuit) {return _literal;}
};

class CTwoLiteralClause : public CClause
{
int _literals[2];

public:
// construct and destruct function
CTwoLiteralClause()
    {
    type() = TWO_LITERAL_CLAUSE;
    }
CTwoLiteralClause(char nflag)
    {
    _type = TWO_LITERAL_CLAUSE;
    _flag = nflag;    
    }
~CTwoLiteralClause() {};

// data member access functions

// memory functions
void* operator new(size_t size);
void operator delete(void* ptr);

virtual int num_literal() {return 2;}
virtual int& literal(int which) {return _literals[which];}

virtual int find_variable_dec(CCircuit& circuit);
virtual bool is_conflict(CCircuit& circuit);
virtual int find_unit_signal(CCircuit& circuit);
virtual int find_max_conflict_dlevel(CCircuit& circuit);
virtual int mark_signal (CCircuit& circuit, int id, int dlevel); 
virtual int unrelevance (CCircuit& circuit);
virtual int find_variable(CCircuit& circuit);
virtual int find_variable_all(CCircuit& circuit);
virtual int find_variable_old(CCircuit& circuit);
virtual void disconnect(CCircuit& circuit);


virtual int find_variable_res(CCNFCircuit& circuit);
virtual int find_variable_dec(CCNFCircuit& circuit);
virtual bool is_conflict(CCNFCircuit& circuit);
virtual int find_unit_signal(CCNFCircuit& circuit);
virtual int find_max_conflict_dlevel(CCNFCircuit& circuit);
virtual int mark_signal (CCNFCircuit& circuit, int id, int dlevel); 
virtual int unrelevance (CCNFCircuit& circuit);
virtual int find_variable(CCNFCircuit& circuit);
virtual int find_variable_all(CCNFCircuit& circuit);
virtual void disconnect(CCNFCircuit& circuit);
virtual CClause* optimize(CCNFCircuit& circuit);
};

class CWatchedClause : public CClause
{
int* _pliteral;
int _num_literal;
int* _pwatched[2];
int  _watched[2];
char _dir[2];

int _used_count;
int _watch_sat;

public:
// constructors & destructors 
CWatchedClause()
    {
    _type = WATCHED_CLAUSE;
    _watch_sat = -1;
    _used_count = 0;
    }
CWatchedClause(short nflag)
    {
    _type = WATCHED_CLAUSE;
    _flag = nflag;
    _watch_sat = -1;
    _used_count = 0;
    }
~CWatchedClause(){}

// data member access functions 
int*& pliteral() {return _pliteral;}
int*& pwatched(short which) {return _pwatched[which];} 
int& watch_sat() {return _watch_sat;} 
int& watched(short which) {return _watched[which];} 
char& dir(short which) {return _dir[which];}
// memory functions
void* operator new(size_t size);
void operator delete(void* ptr);
//
void setup(int* pfirst_literal, int num)
    {
    _pliteral = pfirst_literal;
    _num_literal = num;
    }
void set_watched(int i, char dir, short which)
    {
    _pwatched[which] = _pliteral + i;
    _dir[which] = dir;
    _watched[which] = *(_pliteral+i);
    assert(*(_pwatched[which]) == _watched[which]);
    }
void set_watched(int* ppos,char dir, short which)
    {
    _pwatched[which] = ppos;
    _dir[which] = dir;
    _watched[which] = *ppos;
    assert(*(_pwatched[which]) == _watched[which]);
    }
void remove (CCircuit& circuit);

virtual int find_frontier_variable(CCircuit& circuit, vector<int>& vfrontier);
virtual int num_literal() {return _num_literal;}
virtual int& literal(int which) {return *(_pliteral + which);}
virtual int used_count() {return _used_count;}

virtual int find_variable_dec(CCircuit& circuit);
virtual bool is_conflict(CCircuit&);
virtual int find_unit_signal(CCircuit& circuit);
virtual int find_max_conflict_dlevel(CCircuit& circuit);
virtual int mark_signal (CCircuit& circuit, int id, int dlevel); 
virtual int unrelevance (CCircuit& circuit);
virtual int find_variable(CCircuit& circuit);
virtual int find_variable_all(CCircuit& circuit);
virtual int find_variable_old(CCircuit& circuit);
virtual void disconnect(CCircuit& circuit);

void remove (CCNFCircuit& circuit);
virtual int find_variable_res(CCNFCircuit& circuit);
virtual int find_variable_dec(CCNFCircuit& circuit);
virtual bool is_conflict(CCNFCircuit&);
virtual int find_unit_signal(CCNFCircuit& circuit);
virtual int find_max_conflict_dlevel(CCNFCircuit& circuit);
virtual int mark_signal (CCNFCircuit& circuit, int id, int dlevel); 
virtual int unrelevance (CCNFCircuit& circuit);
virtual int find_variable(CCNFCircuit& circuit);
virtual int find_variable_all(CCNFCircuit& circuit);
virtual void disconnect(CCNFCircuit& circuit);
virtual CClause* optimize(CCNFCircuit& circuit);
};

extern int LookupTableWatched[3][2];

class CThreeLiteralClause : public CClause
{
int _literals[3];
int _not_watched;

public:
// construct and destruct function
CThreeLiteralClause()
    {
    type() = THREE_LITERAL_CLAUSE;
    }
CThreeLiteralClause(char nflag)
    {
    _type = THREE_LITERAL_CLAUSE;
    _flag = nflag;    
    }
~CThreeLiteralClause() {};

// data member access functions

// memory functions

void* operator new(size_t size);
void operator delete(void* ptr);
virtual int num_literal() {return 3;}
virtual int& literal(int which) {return _literals[which];}
int& not_watched() {return _not_watched;}

virtual int find_frontier_variable(CCircuit& circuit, vector<int>& vfrontier);
virtual int find_variable_dec(CCircuit& circuit);
virtual bool is_conflict(CCircuit& circuit);
virtual int find_unit_signal(CCircuit& circuit);
virtual int find_max_conflict_dlevel(CCircuit& circuit);
virtual int mark_signal (CCircuit& circuit, int id, int dlevel); 
virtual int unrelevance (CCircuit& circuit);
virtual int find_variable(CCircuit& circuit);
virtual int find_variable_old(CCircuit& circuit);
virtual void disconnect(CCircuit& circuit);


virtual int find_variable_res(CCNFCircuit& circuit);
virtual int find_variable_dec(CCNFCircuit& circuit);
virtual bool is_conflict(CCNFCircuit& circuit);
virtual int find_unit_signal(CCNFCircuit& circuit);
virtual int find_max_conflict_dlevel(CCNFCircuit& circuit);
virtual int mark_signal (CCNFCircuit& circuit, int id, int dlevel); 
virtual int unrelevance (CCNFCircuit& circuit);
virtual int find_variable(CCNFCircuit& circuit);
virtual void disconnect(CCNFCircuit& circuit);
virtual CClause* optimize(CCNFCircuit& circuit);
};


struct S_clausehash
{
int operator() (CClause* cls, const int size)
    {
    int result = 0;
    int tmpand = ~0;
    int tmpor = 0;
    result += cls->num_literal();
    for (int i = 0; i < cls->num_literal();i++)
        {
        tmpand = tmpand & cls->literal(i);
        tmpor = tmpor | cls->literal(i);
        result += cls->literal(i);
        }
    result += tmpand + tmpor;
    return abs(result%size);
    }
};

struct S_clausecpr
{
bool equal(CClause* c1, CClause* c2)
    {
    if (c1->num_literal() != c2->num_literal())
        return false;
    for (int i = 0; i < c1->num_literal();i++)
        {
        if (c1->literal(i) != c2->literal(i))
            return false;
        }
    return true;
    }
};

NEW(CUniqueClause, gmng._unique_clause);
DELETE(CUniqueClause, gmng._unique_clause);

NEW(CTwoLiteralClause, gmng._two_literal_clause);
DELETE(CTwoLiteralClause, gmng._two_literal_clause);

NEW(CWatchedClause, gmng._watched_clause);
DELETE(CWatchedClause, gmng._watched_clause);

NEW(CThreeLiteralClause, gmng._three_literal_clause);
DELETE(CThreeLiteralClause, gmng._three_literal_clause);
#endif
