
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

// ******************************************************** //
// [GetLongOpt Class] GetLongOpt.h
// 
// Modified by Kai Yang (kyang@ece.ucsb.edu) 2003/July/26
// ******************************************************** //

using namespace std;

#ifndef _GetLongOpt_h_
#define _GetLongOpt_h_

#include <iostream>
#include <string>
#include <set>

class GetLongOpt 
{
public:
    enum OptType { NoValue, OptionalValue, MandatoryValue };

private:
    struct Cell {
        const char *option;  		// option name
        OptType type;     			// option type
        const char *description;   	// a description of option
        const char *value;   		// value of option (string)
        Cell *next;    				// pointer to the next cell

        Cell() { option = description = value = 0; next = 0; }
    };

private:
    Cell *table;          // option table
    const char *ustring;  // usage message
    char *pname;          // program basename
    char optmarker;       // option marker

    int enroll_done;      // finished enrolling
    Cell *last;           // last entry in option table
	set<const char*> ck;  // check duplication

private:
    char *basename(char * const p) const;
    int setcell(Cell *c, char *valtoken, char *nexttoken, char *p);

public:
    GetLongOpt(const char optmark = '-');
    ~GetLongOpt();

    int parse(int argc, char * const *argv);
    int enroll(const char * const opt, const OptType t,
               const char * const desc, const char * const val);
    const char *retrieve(const char * const opt) const;

    void usage(ostream &outfile = cout) const;
    void usage(const char *str) { ustring = str; }
};

#endif // _GetLongOpt_h
