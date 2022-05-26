
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

#ifndef CONST_H 
#define CONST_H

#define UNSET (-1)

#define UNKNOWN 2

// for learned gate removal
#define DELETE_SIGNAL_THRESHOLD 100
#define UNRELEVALENCE_THRESHOLD 20
#define INIT_COUNT 100
#define DECREASE_FACTOR 8
// for simulation
#define SIM_END_THRESHOLD 3
#define SIM_RESTART_THRESHOLD 2

#define MAX_LEARN_BACKTRACK 10

// miscellaneous 
#define CONST_ZERO_ID 1
#define CONST_ONE_ID 2
#define FIRST_VAR_ID 1

//
#define NORMAL_NODE 0
#define IS_JNODE 2

#define NO_NEXT_PROBLEM 0
#define NEXT_PROBLEM 1

// for state minimization 
// add by Feng Lu 4.24.2003
#define UNTESTED 0
#define TESTED 1
#define REMOVED  2

#define MASK0 ((int) 1)

//for encoded circuit
#define NOT_ENCODED 		0
#define FIRST_ENCODED 		1
#define OTHER_ENCODED 		2

#define CFTG_KEEP_FOREVER 0
#define CFTG_KEEP_ASSUMED 1
//
#define MAP_UNSTABLE 	0
#define MAP_STABLE 	1
#define MAP_TIME_OUT 	2 

#define NO_MODE 		-1 
#define COMBINATIONAL_MODE  	0
#define SEQUENTIAL_MODE 	1 

#define CSAT_UNDETERMINED     0
#define CSAT_UNSATISFIABLE    1
#define	CSAT_SATISFIABLE      2
#define CSAT_TIME_OUT         3
#define CSAT_FRAME_OUT        4
#define	CSAT_NO_TARGET        5
#define CSAT_ABORTED          6
#define CSAT_SEQ_SATISFIABLE  7

#ifndef _GATE_TYPE_
#define _GATE_TYPE_
enum GateType 
{
CSAT_CONST = 0,  	// constant gate 
CSAT_BPI,		// boolean PI
CSAT_BPPI,   		// bit level PSEUDO PRIMARY INPUT 
CSAT_BAND, 		// bit level AND 
CSAT_BNAND,  		// bit level NAND 
CSAT_BOR,    		// bit level OR 
CSAT_BNOR,   		// bit level NOR 
CSAT_BXOR,   		// bit level XOR 
CSAT_BXNOR,  		// bit level XNOR 
CSAT_BINV,   		// bit level INVERTER 
CSAT_BBUF,   		// bit level BUFFER 
CSAT_BMUX,   		// bit level MUX 
CSAT_BDFF,   		// bit level D-type FF 
CSAT_BSDFF,  		// bit level scan FF 
CSAT_BTRIH,  		// bit level TRISTATE gate with active high control 
CSAT_BTRIL,  		// bit level TRISTATE gate with active low control 
CSAT_BBUS,   		// bit level BUS
CSAT_BPPO,   		// bit level PSEUDO PRIMARY OUTPUT 
CSAT_BPO,  		// boolean PO
CSAT_BCNF,  		// boolean constraint in CNF format
CSAT_BDC,		// boolean don't care gate (2-input)
CSAT_DUMMY,
};
#endif

#define CSAT_SIGN0  1
#define CSAT_SIGN1  2
#define CSAT_SIGN_BOTH 3

#define CSAT_NO_CONFLICT 0
#define CSAT_CONFLICT 1

#endif
