
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

#ifndef CSAT_BASE_H
#define CSAT_BASE_H
using namespace std;
#include <vector>

#define THREE_GET_OTHER(val) (((int)0x03) & val)
#define THREE_GET_LEFT(val) (((int)0x03) & (val>>2))
#define THREE_GET_CURRENT(val) (((int)0x03) & (val>>4))
#define MAKE_THREE(current,left, other) ((((current<<2) | left)<<2) | other)

struct SDLevel
{
int _dlevel;

//constructor & destructor
SDLevel() {_dlevel = -1;}
~SDLevel() {};
};

struct SAnte
{
CClause* _ante;

//constructor & destructor
SAnte() {_ante = NULL;}
~SAnte(){};
};

struct SMerged
{
int _merged_ids;

//constructor & destructor
SMerged() {_merged_ids = 0;}
~SMerged(){};
};

struct SSignal
{
char _value;

//constructor & destructor
SSignal() {_value = UNKNOWN;}
~SSignal() {};
};

struct SImplication
{
vector<pair<int,CTwoLiteralClause*> > _vimp[2][2];
vector<CThreeLiteralClause*> _vthreeimp[2][2];
vector<CWatchedClause*>  _vwatched[2][2];

//constructor & destructor
SImplication() {};
~SImplication() {};
};

struct SFlag
{
bool _is_marked;
bool _is_dec;
bool _is_fanin;
bool _selected;
char _flag;
char _in_new_clause;
//constructor & destructor
SFlag()
    {
    _is_marked = false;
    _is_dec = false;
    _is_fanin = false;
    _selected = false;
    _flag = NOT_ENCODED;
    _in_new_clause = -1;
    }
~SFlag() {};
};

struct SCandidate
{
bool _learn;
vector<pair<int,bool> >* _candidates;
int _pos;

//constructor & destructor
SCandidate()
    {
    _learn = true;
    _candidates = NULL;
    }
~SCandidate() {};
};

struct SScore
{
int _ac;
int _literal_ac[2];

int _score[2];
int _cftcount[2];
int _prevcount[2];
int _dec_pos;

//constructor & destructor 
SScore() {init(); _literal_ac[0] = 0;  _literal_ac[1] = 0;}
~SScore(){};

// 
void init()
    {
    _ac = 0;
    //_literal_ac[0] = 0;
    //_literal_ac[1] = 0;

    _cftcount[0] = 0;
    _cftcount[1] = 0;
    _prevcount[0] = 0;
    _prevcount[1] = 0;
    _score[0] = 0;
    _score[1] = 0;
    
    }
};


struct SStatus
{
int _num_implications;
int _num_decisions;
int _num_total_imp;
int _num_total_dec;
int _num_backtrack;
bool _restart;
int _restart_num;
bool _disable_restart;
int _num_backlevel;
int _num_period_back;
int _pair_learned;
int _stuck_learned;
long _simulate_time;
long _preprocess_time;
long _solve_time;
long _previous_cftg;
long _current_cftg;
int _num_learn_success;
int _num_learn_unequal;
int _num_learn_abort;
int _common_cftg_begin;
int _common_cftg_count;
int _remove_size;
int _num_cont_poor_restart;
int _num_period_add_clause;
int _num_subproblem_clause;
int _num_objective_clause_added;
int _num_objective_literal_added;
SStatus()
    {
    _num_decisions = 0;
    _num_implications = 0;
    _num_total_dec = 0;
    _num_total_imp = 0;
    _num_backtrack = 0;
    _restart = false;
    _restart_num = 0;
    _disable_restart = false;
    _num_backlevel = 0;
    _num_period_back = 0;
    _pair_learned = 0;
    _stuck_learned = 0;
    _simulate_time = 0;
    _preprocess_time = 0;
    _solve_time = 0;
    _num_learn_success = 0;
    _num_learn_unequal = 0;
    _num_learn_abort = 0;
    _previous_cftg = 0;
    _current_cftg = 0;
    _common_cftg_begin = 0;
    _common_cftg_count = 0;
    _remove_size = 8;
    _num_cont_poor_restart = 0;
    _num_period_add_clause = 0;
    _num_objective_clause_added = 0;
    _num_objective_literal_added = 0;
    }
void init()
    {
    _num_decisions = 0;
    _num_implications = 0;
    _num_backtrack = 0;
    _restart = false;
    _num_backlevel = 0;
    _num_period_back = 0;
    //_common_cftg_begin = 0;
    _common_cftg_count = 0;
    _remove_size = 8;
    _num_cont_poor_restart = 0;
    _num_period_add_clause = 0;
    _current_cftg = 0;
    _previous_cftg = 0;
    _disable_restart = false;
    _num_objective_clause_added = 0;
    _num_objective_literal_added = 0;
    }
};


struct SCNFImplication
{
vector<pair<int,CTwoLiteralClause*> > _vimp[2][2];
vector<CThreeLiteralClause*> _vthreeimp[2][2];
vector<CWatchedClause*>  _vwatched[2][2];

//constructor & destructor
SCNFImplication() {};
~SCNFImplication() {};
};

struct SCNFFlag
{
bool _is_marked;
char _in_new_clause;
//constructor & destructor
SCNFFlag()
    {
    _is_marked = false;
    _in_new_clause = -1;
    }
~SCNFFlag() {};
};

struct SCNFScore
{
int _ac;
int _literal_ac[2];

//constructor & destructor
SCNFScore() {init();}
~SCNFScore(){};

//
void init()
    {
    _ac = 0;
    _literal_ac[0] = 0;
    _literal_ac[1] = 0;
    }
};

struct SCNFDecClauses
{
vector<CClause*> _vclause[2]; 
};
#endif
