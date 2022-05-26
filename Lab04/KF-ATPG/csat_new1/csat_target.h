
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

#ifndef CSAT_TARGET_H
#define CSAT_TARGET_H
using namespace std;

#include <assert.h>
#include <vector>
#include <string>
#include <iostream>
#include "csat_memory.h"
#include "csat_const.h"
#include "csat_macro.h"
#include "csat_netlist.h"


class CTarget
{
vector<int> _vprop;
int _result;
int _no;
int _index;
int _solve_type;
vector<int> _values;
vector<int> _values_ppi;
vector<int> _vcft_reason;
vector<vector<int> > _solution;
int _num_dec;
int _num_imp;
double _time;
int _num_cftg;
int _num_cfts;
int _priority;
int _count;

public:

// construct & destruct
CTarget()
    {
    _result = CSAT_UNDETERMINED; 
    _num_dec = 0;
    _num_imp = 0;
    _time = 0;
    _no = 0;
    _count = 0;
    _priority = 0;
    _index = 0;
    _solution.clear();
    }
~CTarget() {}

// data member access functions
vector<int>& vprop() {return _vprop;}
int& result() {return _result;}
int& no() {return _no;}
int& index() {return _index;}
vector<int>& values() {return _values;}
vector<int>& values_ppi() {return _values_ppi;}
vector<int>& vcft_reason() {return _vcft_reason;}
int& num_dec() {return _num_dec;}
int& num_imp() {return _num_imp;}
int& num_cftg() {return _num_cftg;}
int& num_cfts() {return _num_cfts;}
int& priority() {return _priority;}
int& count() {return _count;}
int& solve_type() {return _solve_type;}
double& time() {return _time;}
vector<vector<int> >& solution() {return _solution;}

// add property
void add_prop(int sigid, int value)
    {
    _vprop.push_back(PACKID_VALUE(sigid,value));
    }
void retarget(CCircuit& circuit);
bool is_target_sat(CCircuit& circuit);
bool is_target_reached(CCircuit& circuit);
void output_target(ostream& os,CCircuit& circuit);
// memory functions
void* operator new(size_t size);
void operator delete(void* ptr);
};

struct S_Target
{
vector<pair<string,int> > _vprop;
bool _is_translated;

S_Target()
    {
    _is_translated = false;
    }
};

NEW(CTarget, gmng._target)
DELETE(CTarget, gmng._target)

#endif

