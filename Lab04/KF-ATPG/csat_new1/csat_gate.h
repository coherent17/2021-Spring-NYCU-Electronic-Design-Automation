
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

#ifndef CSAT_GATE_H
#define CSAT_GATE_H
using namespace std;

#include <assert.h>
#include <string>
#include <vector>
#include <stack>
#include "csat_clause.h"
#include "csat_optimize.h"
#include "csat_literal_pool.h"
#include "csat_resolution.h"

class CCircuit;
class CGate
{
protected:

string _name;
int _type;
int _idx; // index of the gate 
int _merged_ids;
vector<int> _vout;
vector<int> _vout_cnf;
vector<int> _vin;
vector<CClause*> _vclause;
CClause* _nclause;

SOPGate* _opgate;
vector<int> _vfanin;
vector<int> _vtfanin;
vector<S_Resolution*> _vresolution;
public:

// constructor and destructor
CGate(int idx) {_idx = idx; _merged_ids = 0; _opgate = NULL;_nclause = NULL;}
CGate(CGate* g)
    {
    _name = g->name();
    _vout = g->vout();
    _vin = g->vin();
    _merged_ids = 0;
    _opgate = NULL;
    _nclause = NULL;
    }    
~CGate() {};

// memeber data access function
string& name() {return _name;}
int& type() {return _type;}
int& idx() {return _idx;}
vector<int>& vout() {return _vout;}
vector<int>& vout_cnf() {return _vout_cnf;}
vector<int>& vin() {return _vin;}
vector<int>& vfanin() {return _vfanin;}
vector<int>& vtfanin() {return _vtfanin;}
vector<CClause*>& vclause() {return _vclause;}
int& merged_ids() {return _merged_ids;}
CClause* & nclause() {return _nclause;}
SOPGate*& opgate() {return _opgate;}
vector<S_Resolution*>& vresolution() {return _vresolution;}
// valid check function
virtual void check(){assert(_idx == 0);}
virtual int num_signal(){return 1;}
virtual void output_bench(ostream& os, CCircuit& circuit);
virtual void output_bench_inv(ostream& os, CCircuit& circuit);

// structure modify function
void disconnect(CCircuit& circuit);
void remove_fanin(CCircuit& circuit);
void remove_fanin_cnf(CCircuit& circuit);
void replace(int sid, int did, bool sign);
void replace_fanin(CCircuit& circuit, vector<int>& vfanin);
void make_fanout(vector<CGate*>& vcg);
void update_merged_fanin(CCircuit& circuit);
// cut 2 optimization
int cut_2_in(int which);
int cut_2_type(bool is_inv);

//
virtual bool optimize_remodel(CCircuit& circuit) 
    {
    if (_vin.size() == 0)
	{
        _vtfanin.push_back(PACKID_SIGN(_idx,0));
        _vfanin.push_back(PACKID_SIGN(_idx,0));
        }
    else
        {
        _vtfanin = _vin;
        _vfanin = _vin;
        }
    }
virtual void mark_gate_from_po(CCircuit& circuit);
//
virtual void generate_clauses(CCircuit& circuit, SLiteralPool& pool) {};
virtual void regenerate_clauses(CCircuit& circuit, SLiteralPool& pool) {};
};

class CConst : public CGate
{
public:
//constructor
CConst(int idx): CGate(idx){_type = CSAT_CONST;}
CConst(CConst* constg): CGate(constg) {_type = CSAT_CONST;}
// valid check function
virtual void check() {assert((_idx = CONST_ONE_ID) || (_idx = CONST_ZERO_ID));}
};

class CPi : public CGate
{
public:
//constructor
CPi(int idx): CGate(idx){_type = CSAT_BPI;}
CPi(CPi* pi): CGate(pi) {_type = CSAT_BPI;}

// valid check function
virtual void check() 
    {
    assert(_idx > CONST_ONE_ID); 
    assert(_vin.size() == 0);
    }
};

class CPPi : public CPi
{
bool _is_connected;
char _init_value;
int _ppo_idx;
public:
//constructor
CPPi(int idx): CPi(idx){_type = CSAT_BPPI;}
CPPi(CPPi* ppi): CPi(ppi) 
    {
    _type = CSAT_BPPI; 
    _is_connected = ppi->is_connected();
    _init_value = ppi->init_value();
    _ppo_idx = ppi->ppo_idx();
    }
//member access function
bool& is_connected() {return _is_connected;}
char& init_value() {return _init_value;}
int& ppo_idx() {return _ppo_idx;}

// valid check function
virtual void check() 
    {
    assert(_idx > CONST_ONE_ID); 
    if (_is_connected)
	assert(_vin.size() == 1);
    else
	assert(_vin.size() == 0);
    }
 
};

class CPo : public CGate
{
public:
//constructor
CPo(int idx): CGate(idx){_type = CSAT_BPO;}
CPo(CPo* po): CGate(po) {_type = CSAT_BPO;}
 
// valid check function
virtual void check() 
    {
    assert(_vin.size() == 1);
    }
virtual void mark_gate_from_po(CCircuit& circuit) ;
virtual void generate_clauses(CCircuit& circuit, SLiteralPool& pool);
virtual void regenerate_clauses(CCircuit& circuit, SLiteralPool& pool);
};

class CPPo : public CPo
{
int _ppi_idx;
public:
//constructor
CPPo(int idx): CPo(idx){_type = CSAT_BPPO;}
CPPo(CPPo* ppo): CPo(ppo) {_type = CSAT_BPPO; _ppi_idx = ppo->ppi_idx();}
//member access function
int& ppi_idx() {return _ppi_idx;}
// valid check function
virtual void check() 
    {
    assert(_vin.size() == 1);
    }
};

class CAnd : public CGate
{
public:
//constructor
CAnd(int idx): CGate(idx){_type = CSAT_BAND;}
CAnd(CAnd* andg): CGate(andg) {_type = CSAT_BAND;}
// valid check function
virtual void check() 
    {
    assert(_vin.size() > 1);
    }
// optimize function
void optimize(CCircuit& circuit, vector<int>& vflag);
void optimize_s(CCircuit& circuit, vector<int>& vflag);
void optimize_input(CCircuit& circuit);
//
virtual bool optimize_remodel(CCircuit& circuit);
virtual void mark_gate_from_po(CCircuit& circuit) ;
virtual void generate_clauses(CCircuit& circuit, SLiteralPool& pool);
virtual void regenerate_clauses(CCircuit& circuit, SLiteralPool& pool);

};

class CCNF : public CGate
{
public:
CCNF(int idx): CGate(idx){_type = CSAT_BCNF;}
CCNF(CCNF* cnf): CGate(cnf) {_type = CSAT_BCNF;}

void optimize(CCircuit& circuit, vector<int>& vflag);
void optimize_s(CCircuit& circuit, vector<int>& vflag);
virtual void generate_clauses(CCircuit& circuit, SLiteralPool& pool);
virtual void regenerate_clauses(CCircuit& circuit, SLiteralPool& pool);
};

class CXOR: public CGate
{
public:
CXOR(int idx): CGate(idx){_type = CSAT_BXOR;}
CXOR(CXOR* xorg): CGate(xorg) {_type = CSAT_BXOR;}

virtual void generate_clauses(CCircuit& circuit, SLiteralPool& pool);
};
#endif
