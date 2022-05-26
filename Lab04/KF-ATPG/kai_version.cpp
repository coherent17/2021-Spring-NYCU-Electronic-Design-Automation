#pragma GCC diagnostic ignored "-Wwrite-strings"
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

#ifndef CUR_DATE
#define CUR_DATE	"<compile date not supplied>"
#endif

#define CUR_VER "UC Santa Barbara, \"KF-ATPG\" Path-Delay-ATPG v2.0"

#include <stdio.h>
#include <stdlib.h>

static char *proc_date(char *datestr)
{
    static char result[25];
    char day[10], month[10], zone[10], *at;
    int date, hour, minute, second, year;

    if(sscanf(datestr, "%s %s %2d %2d:%2d:%2d %s %4d",
	   day, month, &date, &hour, &minute, &second, zone, &year)==8) {
		if(hour >= 12) {
	    	if (hour >= 13) hour -= 12;
	    at = "PM";
		}
		else {
	    	if (hour == 0) hour = 12;
	    at = "AM";
		}
		(void) sprintf(result, "%d-%3s-%02d at %d:%02d %s", date, month, year % 100, hour, minute, at);
		return result;
    } 
	else {
		return datestr;
    }
}


void Version()
{
    printf("%s (compiled %s by Lu Feng and Kai Yang)\n", CUR_VER, proc_date(CUR_DATE));
}
