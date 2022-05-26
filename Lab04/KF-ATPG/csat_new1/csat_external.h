
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
#ifndef CSAT_EXTERNAL_H
#define CSAT_EXTERNAL_H

#include <stdio.h>
#include "csat_netlist.h"
#include "csat_option.h"

extern FILE *yyin;
extern SNetlist network;
extern ostream& out_to_bench(ostream& os, SGate* g);
extern ostream& out_to_bench(ostream& os, vector<SGate*>& vg);
extern bool read_circuit(string ckt_fname); //read_circuit.cpp
extern bool read_constraint(string constraint_fname); //read_circuit.cpp
extern int yyparse (void);
extern inline bool compare_level(const SGate* g1, const SGate* g2);
extern ostream & operator << ( ostream& os, vector<CGate*>& vcg);
extern ostream & operator << ( ostream& os, vector<int>& vint);
extern long current_cpu_time(); 

extern char inv_tab[2][3];
extern int  and_simulation_table[2][3][2][3];
extern int  BITMASK[32];
#endif
