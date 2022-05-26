
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
// Author: Kai Yang (kyang@ece.ucsb.edu)

using namespace std;

#ifndef KAITYPEEMU_H
#define KAITYPEEMU_H

enum VALUE { S0=0, S1, X, ILLIGAL };

enum GATEFUNC { G_PI=0, G_PO, G_NOT, G_AND, G_NAND, G_OR, G_NOR, G_DFF, G_BUF, G_XOR, G_BAD, G_CNF };
//              0       1     2      3      4       5     6      7      8      9      10	 11
const VALUE NotTable[]={ S1, S0, X, ILLIGAL };
const VALUE CV[]={ X, X, X, S0, S0, S1, S1, X, X, X, X, X };
const VALUE NCV[]={X, X, X, S1, S1, S0, S0, X, X, X, X, X };

#endif
