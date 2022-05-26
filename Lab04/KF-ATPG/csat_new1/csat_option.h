
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

#ifndef CSAT_OPTION_H
#define CSAT_OPTION_H
using namespace std;
#include <string>
#include "csat_const.h"
struct SOption
{
string _program;
string _option;
bool   _use_3_value;
string _ckt_fname;
string _constraint_fname;
double _restart_threshold;
bool _explicit_learning;
int _explicit_learning_size;
double  _learn_stop_ratio;
double  _min_learn_unequal;
int _time_limit;
bool _base_line;
bool _restart;
int _learn_limit;
int _poor_restart_limit;
int _max_learned_cftg_size_kept;
bool _solution;
bool _dump;
string _dump_file;
int _run;
int _debug;
int _dec_mode;
bool _use_resolution;
int _file_format;
string _sym_file;
int _function;

SOption()
    {
    _use_3_value = false;
    _constraint_fname = "";
    _restart_threshold = 1.5;
    _explicit_learning = false;
    _explicit_learning_size = 3;
    _learn_stop_ratio = 2;
    _min_learn_unequal = 50;
    _time_limit = 0;
    _base_line = true;
    _restart = true;
    _learn_limit = MAX_LEARN_BACKTRACK;
    _poor_restart_limit = 3;
    _max_learned_cftg_size_kept = 4;
    _solution = false;
    _dump = false;
    _run = 0;
    _debug = 0;
    _dec_mode = 0;
    _use_resolution = false;
    _file_format = 0;
    _sym_file = "";
    _function = 0;
    }
void get_opt(int argc,char** argv);
void print_usage(int argc,char** argv);
};


#endif

