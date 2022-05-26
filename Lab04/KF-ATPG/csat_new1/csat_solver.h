
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

#ifndef CSAT_SOLVER_H
#define CSAT_SOLVER_H
using namespace std;
#include <queue>
#include "csat_hash.h"
#include "csat_gate.h"
#include "csat_clause.h"
#include "csat_target.h"
#include "csat_optimize.h"
#include "csat_structure.h"
#include "csat_option.h"
#include "csat_util.h"
#include "csat_netlist.h"
#include "csat_literal_pool.h"
#include "csat_external.h"
#include "csat_resolution.h"
#include "csat_circuit.h"
#include "csat_cnf_circuit.h"

class CSolver
{
SNetlist _netlist_org;
SNetlist _netlist_trs;
CCircuit _circuit;
CCNFCircuit _cnf_circuit;
SOption _option;
vector<CTarget*> _vtarget;
vector<S_Target> _vstarget;
CTime		_timer;
public:
SNetlist& netlist_org() {return _netlist_org;} 
SNetlist& netlist_trs() {return _netlist_trs;} 
CCircuit& circuit() {return _circuit;} 
CCNFCircuit& cnf_circuit() {return _cnf_circuit;} 
vector<CTarget*>& vtarget() {return _vtarget;};
vector<S_Target>& vstarget() {return _vstarget;};

void get_opt(int argc, char** argv)
    {
    option().get_opt(argc,argv);
    }
SOption& option()
    {
    return _option;
    }
int read_circuit(string ckt_fname);
int read_constraint(string constraint_fname);
bool init_netlist_org();
void translate_targets();
void add_target(CTarget* target)
    {
    _vtarget.push_back(target);
    }
int solve_target(CTarget* target);
int solve_targets();
void output(int flag, CTarget* target);

//interface function
int add_starget(int nog, char** gates, int* values);
int add_gate(GateType type, char* name, int nofi, char** fanins);
void clear_targets();

// use CNF solver
void init_cnf_solver_by_circuit();
void init_cnf_target(CTarget * target);
int solve_cnf_target(CTarget* target);

void circuit_solve();
void cnf_solve();
};

#endif
