
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

#ifndef CSAT_CIRCUIT_H
#define CSAT_CIRCUIT_H
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

class CCircuit
{
string _cktname;
SOption _option;
string _comment;
SStatus _status;
CTime _timer;
vector<CTarget*> _vtarget;

vector<int> _vflag;
SLiteralPool _original_pool; // original pool for clauses 
SLiteralPool _cft_pool; // conflict pool for clauses  
SLiteralPool _reso_pool; // conflict pool for clauses  

bool _is_unlimited_implication;
bool _use_original_clause_only;
bool _sat;
// circuit network 
// simulation model
vector<CGate*> _vconst;
vector<CGate*> _vpi;
vector<CGate*> _vpo;
vector<CGate*> _vppi;
vector<CGate*> _vppo;
vector<CCNF*> _vconstrain; 
vector<CGate*> _vg; 

// signals information
// implication model
vector<SSignal> _vsignal;
vector<SDLevel> _vdlevel;
vector<SAnte> _vante;
vector<SImplication> _vimplication;
vector<SFlag> _vsflag;
vector<SCandidate> _vsig_candidate;
vector<SScore> _vscore;

//for explicit learning and implicit learning

vector<int> _values;
vector<vector<int> > _vpi_values;
vector<vector<pair<int,bool> >* > _vcandidates;
vector<vector<pair<int,bool> >* > _vff_candidates;
vector<vector<pair<int,bool> >* > _vassumed_eq;
vector<vector<pair<int,bool> >* > _vassumed_eq_unsat;
vector<vector<pair<int,bool> >* > _vassumed_eq_sat;
vector<vector<pair<int,bool> >* > _vassumed_eq_lazy;
vector<pair<int,bool> >* _stuck_at;
char _stuck_at_value;
vector<int> _property;
vector<int> _imply_value[2];
bool _learn_stop;
bool _has_new_partition;
bool _simulation_end;
int _sim_count;
bool _sat_by_simulation;

//circuit optimization
hash_cpr<CAnd*, vector<CAnd*>*, S_pandhash,S_pandcpr> _and_table;
vector<vector<CAnd*>* > _and_groups;

hash_cpr<CAnd*, vector<CAnd*>*, S_pxorhash,S_pxorcpr> _xor_table;
vector<vector<CAnd*>* > _xor_groups;

hash_cpr<CAnd*, vector<CAnd*>*, S_pandfaninhash,S_pandfanincpr> _andfanin_table;
vector<vector<CAnd*>* > _andfanin_groups;
//dynamic information
int _dlevel;
CTarget* _current_target;
vector<vector<int> *> _vassign; //assignment stack
vector<int> _vlocal_assign[2]; //assignment stack for local implication
vector<int> _vlocal_value[2]; //assignment stack for local implication
queue<pair<int, CClause*> > _implication_queue;

vector<int> _vfanin_cone;
vector<int> _vsimulate;
vector<int> _vfanin_pi;
vector<int> _vfanin_ppi;
vector<pair<int,int> > _vdec;	// decision nodes selection set

// conflict analysis
vector<CClause*>   _conflicts; // conflict gates: usually one
int _num_marked;
//conflict signals for conflict analysis
vector<int>     _conflict_signals; 
//for looking forward
vector<vector<int> > _vforward_conflicts;

// for decision making
int _cftg_check_start;
int _index_start;
bool _back_tracked;
int _search_count;
int _num_solution;

// for resolution
vector<S_Resolution*> _vnew_resolution;
vector<vector<S_Resolution*> > _vresolutions[2];
hash_cpr<S_Resolution*, S_Resolution*, S_resolutionhash,S_resolutioncpr> _resolution_table;
//optimize based on structure
public:

// constructor
CCircuit() 
    {
    _dlevel = 0;
    _num_marked = 0;
    _current_target = NULL;
    _cftg_check_start = 0;
    _index_start = 0;
    _back_tracked = true;
    _is_unlimited_implication = true;
    _use_original_clause_only = false;
    _sat_by_simulation = false;
    _sat = 0;
    _learn_stop = false;
    };
~CCircuit() {};
// member access function
bool sat() {return _sat;}
string& cktname() {return _cktname;}
vector<CGate*>& vconst() {return _vconst;}
vector<CGate*>& vpi() {return _vpi;}
vector<CGate*>& vpo() {return _vpo;}
vector<CGate*>& vppi() {return _vppi;}
vector<CGate*>& vppo() {return _vppo;}
vector<CGate*>& vg() {return _vg;} 
vector<CCNF*>& vconstrain() {return _vconstrain;} 

SLiteralPool& original_pool() {return _original_pool;}
SLiteralPool& cft_pool() {return _cft_pool;}

SOption&  option() {return _option;}
CTime& timer() {return _timer;}
vector<int>& vflag() {return _vflag;}
vector<int>& property() {return _property;}
vector<int>& values() {return _values;}
int& num_marked() {return _num_marked;}
SStatus& status() {return _status;}
int& search_count() {return _search_count;}
vector<SImplication>& vimplication() { return _vimplication;}
int get_merged_ids(int idx) {return _vg[idx]->merged_ids();}
void set_merged_ids(int idx,int ids) {_vg[idx]->merged_ids() = ids;}
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
bool is_dec(int id) {return _vsflag[id]._is_dec;}
bool is_fanin(int id) {return _vsflag[id]._is_fanin;}
char in_new_clause(int id) {return _vsflag[id]._in_new_clause;}
void set_is_dec(int id, bool val) {_vsflag[id]._is_dec = val;}
void set_is_fanin(int id, bool val) {_vsflag[id]._is_fanin = val;}
void set_in_new_clause(int id, char val) {_vsflag[id]._in_new_clause = val;}

void init_score(int id) {_vscore[id].init();}
int score(int id) {return _vscore[id]._ac;}
int oscore(int id) {return (_vscore[id]._score[0] > _vscore[id]._score[1])? _vscore[id]._score[0] : _vscore[id]._score[1];}
int dec_pos(int id) {return _vscore[id]._dec_pos;}; 
void inc_score(int id, int val) {_vscore[id]._ac += val;}
void inc_literal_score(int ids, int val) {_vscore[SIGID(ids)]._literal_ac[SIGN(ids)] += val;}
int literal_ac(int id, int which) {return _vscore[id]._literal_ac[which];}
void set_value(int id, char val) {_vsignal[id]._value = val;}
void set_var_dlevel(int id, int val) {_vdlevel[id]._dlevel = val;}
void set_var_ante(int id, CClause* ante) {_vante[id]._ante = ante;}
CClause* get_var_ante(int id) {return _vante[id]._ante;}
CTarget* & current_target() {return _current_target;}
vector<CClause*>& conflicts() {return _conflicts;}
vector<vector<pair<int,bool> >* >& vcandidates() {return _vcandidates;}
bool is_ppi(int id) 
    {
    return  (_vg[id]->type() == CSAT_BPPI); 
    } 
bool is_ppo(int id)
    {
    return (_vg[id]->type() == CSAT_BPPO);
    }
int get_ppi(int ppo)
    {
    assert(_vg[ppo]->type() == CSAT_BPPO);
    assert((_vg[ppo]->idx() - _vppo[0]->idx() + _vppi[0]->idx()) == (((CPPo*)_vg[ppo])->ppi_idx()));
    return (_vg[ppo]->idx() - _vppo[0]->idx() + _vppi[0]->idx());
    }
int get_ppo(int ppi)
    {
    assert(_vg[ppi]->type() == CSAT_BPPI);
    assert((_vg[ppi]->idx() - _vppi[0]->idx() + _vppo[0]->idx()) == (((CPPi*)_vg[ppi])->ppo_idx()));
    return (_vg[ppi]->idx() - _vppi[0]->idx() + _vppo[0]->idx());
    }
// for simulation
void get_random_pattern()
    {
    for (int i = 0; i < _vpi.size(); i++)
	_values[_vpi[i]->idx()] = (int)random();
    for (int i = 0; i < _vppi.size(); i++)
	_values[_vppi[i]->idx()] = (int)random();
    }
void get_random_pattern(vector<int>& vval1,vector<int>& vval2)
    {
    for (int i = 0; i < _vpi.size(); i++)
	{
	vval1[_vpi[i]->idx()] = (int)random();
	vval2[_vpi[i]->idx()] = vval1[_vpi[i]->idx()];
	}
    }
void get_ppi_value()
    {
    assert(_vppi.size() == _vppo.size());
    for (int i = 0; i < _vppi.size() ;i++)
	_values[_vppi[i]->idx()] = _values[_vppo[i]->idx()];
    }
void get_ppi_value(vector<int>& vval1,vector<int>& vval2)
    {
    assert(_vppi.size() == _vppo.size());
    for (int i = 0; i <_vppi.size(); i++)
	{
	vval1[_vppi[i]->idx()] = vval1[_vppo[i]->idx()];
	vval2[_vppi[i]->idx()] = vval2[_vppo[i]->idx()];
	}
    }
void initialize_ppo()
    {
    for (int i = 0; i < _vppo.size(); i++)
	{
	CPPi* ppi = (CPPi*)_vppi[i];
	assert(ppi->init_value() == 0 || ppi->init_value() == 1);
	if (ppi->init_value())
	    _values[_vppo[i]->idx()] = ~((int)0);
	else
	    _values[_vppo[i]->idx()] = 0;
	}
    }
void initialize_ppo(vector<int>& vval1,vector<int>& vval2)
    {
    for (int i = 0; i < _vppo.size(); i++)
	{
	vval1[_vppo[i]->idx()] = ~((int)0);
	vval2[_vppo[i]->idx()] = 0;
	}
    }
bool is_sequential() { if (_vppi.size() >0) return true; else return false;}
void find_correlation();
void generate_candidates(bool is_seq);
void clear_candidates();
void create_candidates();
void simulation();
void partition();
void init_partition();
void further_partition();
bool can_be_partitioned(vector<pair<int,bool> >& candidates);
void partition_candidates(vector<vector<pair<int,bool> >* > & new_vcandidates,vector<pair<int, bool> >& candidates);
void output_sat_simulation(ostream& os, CTarget& target, int result);
void simulate_sat();
void constrain_by_assumed_eq(vector<vector<pair<int,bool> >* >& vgroup);
void ff_init_partition();
void ff_partition();
void equivalence_reasoning();
int equivalence_for_a_signal(int sigid, bool sign);
void check_dec_flag();
void stuck_learning(int sigid);
void set_unlearn_flag(int sigid);
int learn_solve_hard(int endlevel);
int decide_hard(int endlevel);
int decide_hard1(int endlevel);
void derive_eq_signal(int sigid);
void check_cftg_merged();

// for output test vector
void clear_pi_value();
void record_pi_value();

// general function
int net2circuit(SNetlist& net);
void output_bench();
void output_bench_inv();
void write_cnf();
void output(long starttime,int result);
// create info for solver
void generate_clauses();
void generate_clauses(int old_gsize, int old_csize);
void resize();
CClause* add_clause(int flag, char sel, vector<int>& vliteral, SLiteralPool& pool);
void cnf_model_merge(int sid, int did, bool sign);


// optimize function
void optimize();
void optimize_and();
bool optimize_and_gate_s(int i);
void merge_optimize_and(int sid, int did, bool sign);
int  merge(int sid, int did, bool sign);
void merge_s(int sid, int did, bool sign);
void make_fanout();
void make_fanout_after_optimize();
void optimize_2_cut();
int detect_2_cut(int id);
int optimize_2_cut_2_1(int id,int sigids0,int sigids1,int ids00,int ids01);
int optimize_2_cut_2_2(int id,int sigids0,int sigids1,int ids00,int ids01,int ids10, int ids11);
void mark_gate_from_po();
void regenerate_vg(SNetlist* netlist);
// solver functions
int init_solve_target();
void back_track(int level);
int presolve(bool is_active_target);
int preprocess();
void queue_implication(int ids, CClause* ante)
    {
    assert(SIGID(ids) != 0);
    _implication_queue.push(pair<int,CClause*>(ids,ante));
    }
int implication();
int implication(int id, char val, CClause* ante);
int implication_watched_clauses(vector<CWatchedClause*>& vwatched, int id,char sel);
int implication_three_clauses(vector<CThreeLiteralClause*>& vimp, int id,char sel);


int implication_count(int id, char val);
int simple_implication(int id, char val, queue<int>& qsig, vector<int>& vsig);
int simple_implication_watched_clauses(vector<CWatchedClause*>& vwatched,
        int id,char sel,queue<int>& qsig, vector<int>& vsig);
int simple_implication_three_clauses(vector<CThreeLiteralClause*>& vimp,
        int id,char sel,queue<int>& qsig, vector<int>& vsig);
int conflict_analysis();
void mark_signal(int sigid, int dl);
int solve_target();
void get_fanin_cone(queue<int>& qsig);
void routine_process(int endlevel);
bool decide_sim();
bool decide_sim1();
void restart(int endlevel);
void update_signal_score_cft();
void remove_according_time();
void remove_learned_clauses();
int find_variable();
int find_frontier_decision();
int nb_num(int id, bool val);
void calculate_initial_literal_ac();

// for looking forward
int get_frontier_variables();
int derive_conflict_clause();
void add_look_forward_clauses();
// special function
void find_articulate();
void find_articulate_gate(int id, int parent_id, int& index, vector<bool>& flags, vector<bool>& vres, vector<int>& vindex, vector<int>& vback);
void generate_saucy_graph();
void generate_cnf_saucy_graph();
void generate_symmetry_clause(SaucySymmetries& syms);
void generate_symmetry_clause(SaucySymmetry& syms, int old_c, int old_g);
// resolution funtions
void resolution();
void init_gate_resolution(int id);
void init_resolution();
void init_clause_resolution(CClause* clause,vector<int>& vused_id);
void add_gate_resolution(int id);
void perform_resolution(vector<S_Resolution*>& vreso, vector<int>& vused_id);
void resolution_by_gate(int id);
void resolution_by_clause(CClause* clause);
void resolution_clause(CClause* clause);
void generate_resolution(S_Resolution* preso, vector<S_Resolution*>& vgene_reso);
int add_resolution_clause();
int add_static_resolution_clause();
};


#endif
