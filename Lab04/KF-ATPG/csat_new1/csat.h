
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

#ifndef _CSAT_H
#define _CSAT_H

#include "csat_netlist.h"
//CSAT_StatusT defines the return value by CSAT_Solve();

#ifndef CSAT_Manager
#define CSAT_Manager void *
#endif

/*============================================================
This is the header for using the csat solver. A typical flow is

1. Call CSAT_InitManager() to create a new manager;

2. Add gates by calling CSAT_AddGate() to set up the circuit;

3. Optionally, you can check the integrity of circuit by calling 
   CSAT_Check_Integrity();

4. Optionally, you can set the time limit for solving a target;

5. Add targets to be solved by CSAT_AddTarget(), you can add multiple
   targets, these targets will be solved one by one but the learned gates
   will be shared by later targets;

6. Call CSAT_SolveInit() to initilalize the data structures that will be used
   for solving the targets;

7. Call  CSAT_Solve() to solve the targets;

8. Optionally, you can obtain the solving status for a target by calling
   CSAT_Get_Target_Result();

9. Call  CSAT_ReleaseManager() to release the manager;

You also need to link the library libsat.a using c++ linker
***********************************************************************/

// The following are the descriptions of the main functions

// create a new manager
CSAT_Manager CSAT_InitManager(void);

// release a manager
void CSAT_ReleaseManager(CSAT_Manager mng);

// add a gate to the circuit
// the meaning of the parameters are:
// type: the type of the gate to be added
// name: the name of the gate to be added, name should be unique in a circuit.
// nofi: number of fanins of the gate to be added;
// fanins: the name array of fanins of the gate to be added
int CSAT_AddGate(CSAT_Manager mng,
            enum GateType type,
            char* name,
            int nofi,
            char** fanins);

//set solver options
//
void CSAT_SetSolveOption(CSAT_Manager mng, char* name, char* value);

void CSAT_Netlist(CSAT_Manager mng, SNetlist* pnetlist);

// check if there are gates that are not used by any primary ouput.
// if no such gates exist, return 1 else return 0;
int CSAT_Check_Integrity(CSAT_Manager mng);

// add target to be solved 
// the meaning of the parameters are:
// nog: number of gates that are in the targets
// names: name array of gates 
// values: value array of the corresponding gates given in "names" to be 
// solved. the relation of them is AND.  
int CSAT_AddTarget(CSAT_Manager mng, int nog, char**names, int* values);

// clear all the targets defined by CSAT_AddTarget()
void CSAT_ClearTargets(CSAT_Manager mng);

// initialize the solver internal data structure.
void CSAT_SolveInit(CSAT_Manager mng);

// solve the targets added by CSAT_AddTarget()
int CSAT_Solve(CSAT_Manager mng);

// get the solve status of a target
// TargetID: the target id returned by  CSAT_AddTarget().
int CSAT_Get_Target_Result(CSAT_Manager mng, int TargetID);

int CSAT_Name_To_IDS(CSAT_Manager mng, string SignalName);

int CSAT_Signal_Value(CSAT_Manager mng, int ids);

#endif
