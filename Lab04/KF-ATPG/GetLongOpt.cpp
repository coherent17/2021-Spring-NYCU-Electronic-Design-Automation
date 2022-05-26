
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
// [GetLongOpt Class] GetLongOpt.cpp
// 
// Modified by Kai Yang (kyang@ece.ucsb.edu) 2003/July/26
// ******************************************************** //

using namespace std;
#include <cstring> 
#include <cstdlib>
#include "GetLongOpt.h"
#include <iostream>
#include <set>

GetLongOpt::GetLongOpt(const char optmark)
{
    table = last = 0;
    ustring = "[valid options and arguments]";
    enroll_done = 0;
    optmarker = optmark;
}


GetLongOpt::~GetLongOpt()
{
    Cell *t = table;

    while ( t ) {
        Cell *tmp = t;
        t = t->next;
        delete tmp;
    }
}


char* GetLongOpt::basename(char * const pname) const
{
    char *s;

    s = strrchr(pname, '/');
    if ( s == 0 ) s = pname;
    else ++s;

    return s;
}


void GetLongOpt::usage(ostream &outfile) const
{
    Cell *t;

    outfile << "usage: " << pname << " " << ustring << "\n";
    for ( t = table; t != 0; t = t->next ) {
        outfile << "\t" << optmarker << t->option;
        if ( t->type == GetLongOpt::MandatoryValue )
            outfile << " <$val>";
        else if ( t->type == GetLongOpt::OptionalValue )
            outfile << " [$val]";
        outfile << " (" << t->description << ")\n";
    }
    outfile.flush();
}


int GetLongOpt::enroll(const char * const opt, const OptType t,
                   	   const char * const desc, const char * const val)
{
    if ( enroll_done ) {
		cerr << "GetLongOpt::enroll already done!" << endl;
		cerr << "Exit!" << endl;
		exit(-1);
	}

	if ( ck.find(opt) != ck.end() ) {
		cerr << "GetLongOpt::duplicated enroll (" << optmarker << opt << ")" << endl;
		cerr << "Exit!" << endl;
		exit(-1);
	}
	ck.insert(opt);

    Cell *c = new Cell;
    c->option = opt;
    c->type = t;
    c->description = desc ? desc : "no description available";
    c->value = val;
    c->next = 0;

    if ( last == 0 ) {
        table = last = c;
    }
    else {
        last->next = c;
        last = c;
    }

    return 1;
}


const char* GetLongOpt::retrieve(const char * const opt) const
{
    Cell *t;
    for ( t = table; t != 0; t = t->next ) {
        if ( strcmp(opt, t->option) == 0 )
            return t->value;
    }
    cerr << "GetLongOpt::retrieve - unenrolled option ";
    cerr << optmarker << opt << "\n";
    return 0;
}


int GetLongOpt::parse(int argc, char * const *argv)
{
    int optind = 1;

    pname = basename(*argv);
    enroll_done = 1;
    if ( argc-- <= 1 ) return optind;

    while ( argc >= 1 ) {
        char *token = *++argv; --argc;

        if ( token[0] != optmarker || token[1] == optmarker ) {
			cerr << "GetLongOpt::invalid option!" << endl;
			cerr << "Exit!" << endl;
			exit(-1);
		}

        ++optind;

        char *tmptoken = ++token;
        while ( *tmptoken )
            ++tmptoken;

        Cell *t;
        enum { NoMatch, ExactMatch, PartialMatch } 
		matchStatus = NoMatch;
        Cell *pc = 0;  // pointer to the partially-matched cell
        for ( t = table; t != 0; t = t->next ) {
            if ( strcmp(t->option, token ) == 0 ) {
                int stat = setcell(t, tmptoken, *(argv+1), pname);
                if ( stat == -1 ) return -1;
                else if ( stat == 1 ) {
                    ++argv; --argc; ++optind;
                }
                matchStatus = ExactMatch;
                break;
			}
            if ( strncmp(t->option, token, (tmptoken - token)) == 0 ) {
                if ( strlen(t->option) == (unsigned) (tmptoken - token) ) {
                    // an exact match found //
                    int stat = setcell(t, tmptoken, *(argv+1), pname);
                    if ( stat == -1 ) return -1;
                    else if ( stat == 1 ) {
                        ++argv; --argc; ++optind;
                    }
                    matchStatus = ExactMatch;
                    break;
                }
                else {
                    // partial match found //
					if ( matchStatus != NoMatch) {
						cerr << "GetLongOpt::ambiguous partial-match";
						cerr << " (" << token << ")" << endl;
						cerr << "Exit!" << endl;
						exit(-1);
					}
                    matchStatus = PartialMatch;
                    pc = t;
                }
            }
        }

        if ( matchStatus == PartialMatch ) {
            int stat = setcell(pc, tmptoken, *(argv+1), pname);
            if ( stat == -1 ) return -1;
            else if ( stat == 1 ) {
                ++argv; --argc; ++optind;
            }
        }
        else if ( matchStatus == NoMatch ) {
            cerr << pname << ": unrecognized option ";
            cerr << optmarker << strtok(token,"= ") << "\n";
            return -1;    // no match //
        }

    }

    return optind;
}


// GetLongOpt::setcell returns
//   -1 if there was an error
//    0 if the nexttoken was not consumed
//    1 if the nexttoken was consumed

int GetLongOpt::setcell(Cell *c, char *valtoken, char *nexttoken, char *name)
{
    if ( c == 0 ) return -1;

    switch ( c->type ) {
    case GetLongOpt::NoValue :
        c->value = (c->value) ? 0 : (char *) ~0;
        return 0;
    case GetLongOpt::OptionalValue :
        if ( nexttoken != 0 && nexttoken[0] != optmarker ) {
            c->value = nexttoken;
            return 1;
        }
        else return 0;
        break;
    case GetLongOpt::MandatoryValue :
        if ( nexttoken != 0 && nexttoken[0] != optmarker ) {
            c->value = nexttoken;
            return 1;
        }
        else {
            cerr << name << ": mandatory value for ";
            cerr << optmarker << c->option << " not specified\n";
            return -1; // mandatory value not specified //
        }
        break;
    default :
        break;
    }
    return -1;
}
