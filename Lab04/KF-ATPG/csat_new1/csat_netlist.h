
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

#ifndef CSAT_NETLIST_H
#define CSAT_NETLIST_H

using namespace std;

#include <string>
#include <vector>
#include <list>
#include <set>
#include <queue>
#include <algorithm>
#include <iostream>
#include <stdio.h>
#include "csat_util.h"
#include "csat_const.h"

struct SNetlist;

#define CHECK(a) 

// simple netlist structure used to read in benchmark circuit is define here
// other structures needed will be transformed from it.
class CGate;
class CCircuit;

struct SGate
{
string _name;		//gate name
int _idx;		//index
int _type;		//gate type PI, PO, AND ....
vector<SGate*> _vin;	//fanin vector
			// for BMUX, it has 3 input, the first 2 are data
			// the third one is the control, if control == 0
			// the first data would be the output
vector<bool> _vin_sign;//inputs sign  
vector<SGate*> _vout;	//fanout vector
int _level; 		//used for levelize;
bool _flag; 		//general usage

// used for transformation
vector<SGate*> _trans; // gate vector for gates transformed by this gate 
SGate* _output;	// pointer to tranformed gate that corresponding 
			// to the output of this gate 
bool _sign;		// record the sign corresponding between 
			// _output and this gate output, _sign = 1 
			// means they have reverse phase, _sign = 0;
			// means they have the same phase
bool _is_3_value;
short _init_value;	// for flip-flop, 0, 1 or UNKNOWN 
bool _is_first;         // for remodeled 3 value gate,indicate if is the first bit 
int _ids;		// record relation with CGate by id and sign
// constructor & destructor
SGate(int type = CSAT_BPI, string name = "")
    {
    _type = type;
    _name = name;
    _flag = false;
    _output = NULL;
    _is_3_value = false;
    _init_value = 0;
    _is_first = false;
    _level = 0;
    _idx = -1;
    _ids = -1;
    }
SGate(SGate* sg)
    {
    _type = sg->_type;
    _name = sg->_name;
    _output = NULL;
    _flag = false;
    _idx = sg->_idx;
    _vin = sg->_vin;	
    _vin_sign = sg->_vin_sign;	
    _is_3_value = sg->_is_3_value;
    _init_value = sg->_init_value;
    _is_first = sg->_is_first;
    _level = 0;
    _ids = sg->_ids;
    }
~SGate() {};

// memeber data info access function
void set_name(char* s)	{_name = s;}
void set_name(string s)	{_name = s;}
const char* name()	{return _name.c_str();}
string& name_str() {return _name;}

//Miscellaneous function
int levelize();	// levelize the gate list
void translate_and2(SNetlist& net); //translate to and2 gates netlist
void translate_bmux_and2(SNetlist& net); //translate to and2 gates netlist
void translate_bdc_and2(SNetlist& net); //translate to and2 gates netlist
void create_cgate(CCircuit& circuit, vector<int>& vflag); // create CGate of it
void create_cand(CCircuit& circuit, vector<int>& vflag);
void create_ccnf(CCircuit& circuit, vector<int>& vflag);
void check_integrity(SNetlist& netlist);
void check(SNetlist& netlist);
// translate 2 inputs gates to and2 gates
void two_input_and_or_trans_and2(SNetlist& net); 

// translate mult-input gates to and2 gates
void multi_input_and_or_trans_and2(SNetlist& net);
void multi_input_xnor_trans_and2(SNetlist& net);
void multi_input_xor_trans_and2(SNetlist& net);

// traslate multi-input and or gates to multi-input and gate
void translate_and_multi(SNetlist& net); 
void and_or_trans_and_multi(SNetlist& net);
//
int get_cgate_id();
void remodel_ff (SNetlist& net,SGate* ppi);


void update_gate_ids(vector<int>& vnew_index, CCircuit& circuit);

// output function
friend ostream & operator << ( ostream & os, SGate & g);
};

struct SNetlist
{
string	_name;
int _level;
bool _use_3_value;
vector<SGate*> _vconst;	//const vector
vector<SGate*> _vpi;	//pi vector
vector<SGate*> _vpo;	//po vector
vector<SGate*> _vppi;	//ppi vector
vector<SGate*> _vppo;	//ppo vector
vector<SGate*> _vff;	//flip-flop vector
vector<SGate*> _vgate;	//gate vector for other logic gates
vector<SGate*> _vcheckpoint;	//gate vector for other gates

vector<SGate*> _vconstrain;	//gate vector for constraint gates
vector<SGate*> _vbgate; //transformed gate vector;
vector<SGate*> _v3gate; //transformed gate vector for 3 value problem;

hash<string, SGate*, S_strhash> _name_table;
hash<void*, void*, S_ptrhash<void*> > _fl_table;
hash<void*, SGate*, S_ptrhash<void*> > _fl_inv_table;
hash<void*, void*, S_ptrhash<void*> > _sec_table;
vector<void*> _vmap;

hash<SGate*, vector<string>*, S_ptrhash<SGate*> > _fanin_table;

SNetlist()
    {
    _name = "csat_temp";
    _use_3_value = false;
    }
~SNetlist() {};
void clear();
void reset();
void
init()
    {
    _vconst.clear();
    _vpi.clear();	//pi vector
    _vpo.clear();	//po vector
    _vppi.clear();	//ppi vector
    _vppo.clear();	//ppo vector
    _vff.clear();	//flip-flop vector
    _vgate.clear();	//gate vector for other gates
    _vconstrain.clear();
    _vbgate.clear(); //transformed gate vector;
    _name_table.clear();
    _fanin_table.clear();
    }
void setup_fanins(bool use_3_value = false);
void setup_fanins(vector<SGate*>& vg);
void ff_to_ppi_ppo();
void sec_ff_to_ppi_ppo();
int levelize();
void transform_and2(SNetlist& net);
void transform_and2(SNetlist& net, vector<SGate*>& vg);

void rename_signal();
bool check_integrity();
bool check();
int nametoids(string& name);
ostream& out_to_bench(ostream& os);
void extend_frame(int num,SNetlist* net_extended);
void setup_frame();
void setup_fanin(SGate* sg, int nof);
void add_constraint(SNetlist& netlist);
void remodel_3_value();
void redirect_fanin();
void remodel_ff();

void transform_and_multi(SNetlist& net, vector<SGate*>& vg);
void transform_and_multi(SNetlist& net);

void update_gate_ids(vector<int>& vnew_index, CCircuit& circuit);

friend ostream & operator << ( ostream & os, vector<SGate*>& vg);
friend ostream & operator << ( ostream & os, SNetlist & net);
};

#endif
