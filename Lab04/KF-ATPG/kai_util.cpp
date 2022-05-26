
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
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "GetLongOpt.h"
#include <sys/time.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include <cstdlib>

extern GetLongOpt option;
void Version();

int SetupOption(int argc, char ** argv)
{
    option.usage("[options]");

    option.enroll("help", GetLongOpt::NoValue, "print this help summary", 0);
    option.enroll("version", GetLongOpt::NoValue, "print this version", 0);
	option.enroll("circuit", GetLongOpt::MandatoryValue, "set the input circuit file", 0);
	option.enroll("path_t", GetLongOpt::MandatoryValue, "set the path file WITH transition", 0);
	option.enroll("path_not", GetLongOpt::MandatoryValue, "set the path file WITHOUT transition", 0);
	option.enroll("chain", GetLongOpt::MandatoryValue, "set the scan chain order file", 0);
	option.enroll("timeframe", GetLongOpt::MandatoryValue, 
			       "set the # of timeframe for Functional-Justification, default=2", 0);
	option.enroll("constraint", GetLongOpt::MandatoryValue, "set the file of ATPG constraints in CNF form", 0);
	option.enroll("atpg", GetLongOpt::MandatoryValue, "set ATPG strategy NR/SR (Non-Robust/Semi-Robust), default=NR", 0);
	option.enroll("checkpath", GetLongOpt::NoValue, "enable path checking procedure", 0);
	option.enroll("output", GetLongOpt::MandatoryValue, "set output pattern file name", 0);
	option.enroll("unknown", GetLongOpt::MandatoryValue, "set unknown PI value ZERO/ONE/RAND/X/LESS", 0);

    int optind=option.parse(argc, argv);
    if(optind<1) { cerr<<"Need at Least One Command!"<<endl; exit(-1); }

    if(option.retrieve("help")) {
        option.usage();
        exit(-1);
    }
    if(option.retrieve("version")) {
		Version();
        exit(-1);
    }
    return optind;
}


double get_cpu_time()
{
    struct tms buf;
    times(&buf);
    return (double)(buf.tms_utime+buf.tms_stime)/(double)sysconf(_SC_CLK_TCK);
}


double show_time(double old_t, string name)
{
	double new_t=get_cpu_time();
	cout<<"CPU Time Usage for \""<<name<<"\" = "<<new_t-old_t<<endl;
	return new_t;
}
