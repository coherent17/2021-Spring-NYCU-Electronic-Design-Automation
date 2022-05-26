
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

#ifndef CSAT_OPTIMIZE_H
#define CSAT_OPTIMIZE_H
using namespace std;

#include <string>
#include <vector>
#include <stack>

struct SOPGate
{
int _type;
vector<int> _vin;
};
 
#define OP_NO_EXIST		0
#define OP_MERGE_ZERO		1	
#define OP_MERGE_CHILD_1	2	
#define OP_AND_A__B		3	
#define OP_MERGE__A		4	
#define OP_AND__A_B		5	
#define OP_AND_A_B		6
#define OP_AND__A__B		7
#define OP_MERGE_CHILD_2	8
#define OP_MERGE__B		9
#define OP_XOR_AB		10


extern int optimize_table_2_1[4][2];
extern int optimize_table_2_2[4][4][2][2];

#endif
