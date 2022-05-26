/*Scanner for reading benchmark circuit netlist*/

%{
using namespace std;
#include <string>
#include "kai_typeemu.h"
#include "readbench.tab.h"
        
long int kai_lineno=1;
%}

WS [ \t]
STRING [_a-zA-z0-9][_a-zA-z0-9\.]*

%option noyywrap
%%
{WS}     { /* Ignore white space*/ }
^#[^\n]*\n  { /* Ignore Comment */kai_lineno++; }
^\n { kai_lineno++; }
[\n]  { kai_lineno++; return(EOLINE); }
INPUT { return(GINPUT); }
OUTPUT { return(GOUTPUT); }
NOT { return(GNOT); }
AND { return(GAND); }
OR { return(GOR); }
NAND { return(GNAND); }
NOR { return(GNOR); }
DFF { return(GDFF); }
BUF { return(GBUF); }
BUFF { return(GBUF); }
XOR { return(GXOR); }
{STRING} { 
    strcpy(kklval.str, (char *)kktext);
    return(NAMESTRING);
}
"=" { return(EQUAL); }
"(" { return(LPAR); }
")" { return(RPAR); }
"," { return(COMMA); }
.  { printf("Unrecognized symbol error at %s in line %ld\n",yytext, kai_lineno); }
%%
