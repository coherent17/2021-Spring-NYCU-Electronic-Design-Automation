
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

#ifndef CSAT_CNF_CIRCUIT_H
#define CSAT_CNF_CIRCUIT_H
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
#include "csat_base.h"
#include "csat_saucy.h"

class CCNFCircuit
{
SStatus _status;
SOption _option;
CTime _timer;
vector<CTarget*> _vtarget;
string _cktname;
string _comment;
SLiteralPool _original_pool; // original pool for clauses 
SLiteralPool _cft_pool; // conflict pool for clauses  

bool _sat;
// circuit network 
// signals information
// implication model
vector<SSignal> _vsignal;
vector<SDLevel> _vdlevel;
vector<SAnte> _vante;
vector<SMerged> _vmerged;
vector<SCNFImplication> _vimplication;
vector<SCNFFlag> _vsflag;
vector<SCNFScore> _vscore;
vector<SCNFDecClauses> _vdec_clause;

//dynamic information
int _dlevel;
vector<bool> _vchronological_backtrack;
vector<vector<int> *> _vassign; //assignment stack
vector<int> _vlocal_value[2]; //assignment value for local implication
queue<pair<int, CClause*> > _implication_queue;

int  _dec_vindex;
list<CClause*> _vclause_dec;
list<pair<int, int> > _lvar_dec;
vector<pair<int, int> > _vvar_dec;
vector<pair<int, int> > _vimpvalue;
bool _is_clause_sat;
list<pair<int, int> >::iterator _dec_index;

// conflict analysis
vector<CClause*>   _conflicts; // conflict gates: usually one
vector<vector<int> > _vlocal_conflicts;
int _num_marked;
//conflict signals for conflict analysis
vector<int>     _conflict_signals; 
// for decision making
int _cftg_check_start;
int _index_start;
int _dec_pos;
bool _back_tracked;
int _search_count;
vector<pair<int,int> > _vdec;

// local learning 
hash_cpr<CClause*, CClause*, S_clausehash,S_clausecpr> _clause_table;
public:

// constructor
CCNFCircuit() 
    {
    _dlevel = 0;
    _num_marked = 0;
    _cftg_check_start = 0;
    _index_start = 0;
    _back_tracked = true;
    _sat = 0;
    };
~CCNFCircuit() {};
// member access function
bool sat() {return _sat;}
SOption&  option() {return _option;}
CTime& timer() {return _timer;}
string& cktname() {return _cktname;}
int& num_marked() {return _num_marked;}
SLiteralPool& original_pool() {return _original_pool;}
SLiteralPool& cft_pool() {return _cft_pool;}
SStatus& status() {return _status;}
int& search_count() {return _search_count;}
vector<pair<int, int> >& vvar_dec() {return _vvar_dec;} 
vector<SCNFImplication>& vimplication() { return _vimplication;}
int get_merged_ids(int idx) {return _vmerged[idx]._merged_ids;}
void set_merged_ids(int idx,int ids) {_vmerged[idx]._merged_ids = ids;}
char& value(int id) {return _vsignal[id]._value;} 
bool is_marked(int id) {return _vsflag[id]._is_marked;}
void set_is_marked(int id, bool val) {_vsflag[id]._is_marked = val;}

char get_value_s(int id_s)
    {
    return (_vsignal[SIGID(id_s)]._value ^ SIGN(id_s));
    }
char get_value_sign(int id_s)
    {
    return inv_tab[SIGN(id_s)][_vsignal[SIGID(id_s)]._value];
    }
int& get_dlevel(int id) {return _vdlevel[id]._dlevel;}
int& dlevel() {return _dlevel;}
char in_new_clause(int id) {return _vsflag[id]._in_new_clause;}
void set_in_new_clause(int id, char val) {_vsflag[id]._in_new_clause = val;}

void init_score(int id) {_vscore[id].init();}
int score(int id) {return _vscore[id]._ac;}
void inc_score(int id, int val) {_vscore[id]._ac += val;}
void inc_literal_score(int ids, int val) {_vscore[SIGID(ids)]._literal_ac[SIGN(ids)] += val;}
int literal_ac(int id, int which) {return _vscore[id]._literal_ac[which];}
void set_literal_ac(int id, int which, int val) {_vscore[id]._literal_ac[which] = val;}
void set_value(int id, char val) {_vsignal[id]._value = val;}
void set_var_dlevel(int id, int val) {_vdlevel[id]._dlevel = val;}
void set_var_ante(int id, CClause* ante) {_vante[id]._ante = ante;}
CClause* get_var_ante(int id) {return _vante[id]._ante;}
vector<CClause*>& conflicts() {return _conflicts;}
SCNFDecClauses& get_dec_clauses(int id) {return _vdec_clause[id];}
// general function
void output(long starttime,int result);
int read_cnf(string fname);
void generate_saucy_graph();
// create info for solver
void resize(int num);
CClause* add_clause(char flag, char sel, vector<int>& vliteral, SLiteralPool& pool);
void add_clause(char flag, char sel, CClause* clause, SLiteralPool& pool);
void arrange_decision_clauses();
int cnf_optimize(SLiteralPool& pool);
// solver functions
void back_track(int level);
void queue_implication(int ids, CClause* ante)
    {
    assert(SIGID(ids) != 0);
    _implication_queue.push(pair<int,CClause*>(ids,ante));
    }
int implication();
int implication(int id, char val, CClause* ante);
int implication_watched_clauses(vector<CWatchedClause*>& vwatched, int id,char sel);
int implication_three_clauses(vector<CThreeLiteralClause*>& vimp, int id,char sel);

int implication_count(int id, char val, vector<int>& vimplied);
int simple_implication(int id, char val, queue<int>& qsig, vector<int>& vsig);
int simple_implication_watched_clauses(vector<CWatchedClause*>& vwatched,
        int id,char sel,queue<int>& qsig, vector<int>& vsig);
int simple_implication_three_clauses(vector<CThreeLiteralClause*>& vimp,
        int id,char sel,queue<int>& qsig, vector<int>& vsig);
int conflict_analysis();
int conflict_analysis_local();
void local_learn_by_clause();
void local_learn_by_cft_clause();
void detect_relations_from_implications(int count);
int learn_from_clause(CClause* clause, int depth, int& count);
void mark_signal(int sigid, int dl);
int init_solve_target();
int presolve();
void calculate_initial_literal_ac();
void setup_variable_for_dec();
int solve_target();
void routine_process(int endlevel);
bool decide_sim();
bool decide_sim1();
bool decide_sim2();
bool decide_sim3();
bool decide_sim4();
int find_variable_res();
void restart(int endlevel);
void update_signal_score_cft();
void remove_according_time();
int find_variable();
int nb_num(int id, bool val);
void write_cnf();

void generate_symmetry_clause(SaucySymmetries& syms);
void generate_symmetry_clause(SaucySymmetry& sym, int old_s);

};


#endif
