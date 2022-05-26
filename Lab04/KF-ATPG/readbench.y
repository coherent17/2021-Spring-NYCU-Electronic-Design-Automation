/* Parser for reading the benchmark circuit in .bench format */

%{
using namespace std;
#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include "kai_gate.h"
#include "kai_netlist.h"
#include "kai_typeemu.h"
        
extern int kklex(void); 
int kkerror(char *err);
bool ParseError=false;

extern long int kai_lineno;
extern char* kktext;
extern FILE *kkin;

extern KaiNetlist NETLIST;
string name;
namespace kai {
	KaiGATE* gptr=NULL;
}
using kai::gptr;
KaiGATE* inlist=NULL;
extern map<string, KaiGATE*> KaiNameTable;
%}

%union {
long num;
char str[256];
GATEFUNC func;
}


%token <num> GINPUT GOUTPUT GDFF GNOT GAND GOR GNAND GNOR
%token <num> GBUF GXOR
%token EOLINE LPAR RPAR COMMA EQUAL

%token <str> NAMESTRING

%type <func> gatename

%start netlist

%%
netlist: circuit {
};

circuit : 
         input_definition 
         | output_definition 
         | gate_definition 
         | circuit input_definition
         | circuit output_definition
         | circuit gate_definition
;

input_definition: GINPUT LPAR NAMESTRING RPAR EOLINE {
					  if(KaiNameTable.find($3)==KaiNameTable.end()) {
                          gptr=new KaiGATE;
                          gptr->SetName($3);
                          gptr->SetFunction(G_PI);
						  gptr->SetInverse(false);
						  KaiNameTable[$3]=gptr;
                          NETLIST.AddGate(gptr);
                       }
                       else {
						  cerr<<"Duplicated PI"<<endl;
						  exit(-1); 
                       }
                  }
;
        
output_definition: GOUTPUT LPAR NAMESTRING RPAR EOLINE {
                       name=string("PO_")+$3;
					   if(KaiNameTable.find(name)==KaiNameTable.end()) {
                           gptr=new KaiGATE;
                           gptr->SetName(name);
                           gptr->SetFunction(G_PO);
						   gptr->SetInverse(false);
						   KaiNameTable[name]=gptr;
                           NETLIST.AddGate(gptr);
                       }
                       else {
						   cerr<<"Duplicated PO"<<endl;
						   exit(-1); 
                       }

					   if(KaiNameTable.find($3)==KaiNameTable.end()) {
                           inlist=new KaiGATE;
                           inlist->SetName($3);
						   KaiNameTable[$3]=inlist;
                           NETLIST.AddGate(inlist);
                       }
                       else {
						   inlist=(*KaiNameTable.find($3)).second;
                       }
                       gptr->AddFanin(inlist);
                   }
;
        
gate_definition: NAMESTRING EQUAL gatename LPAR {
					   if(KaiNameTable.find($1)==KaiNameTable.end()) {
                           gptr=new KaiGATE;
                           gptr->SetName($1);
						   KaiNameTable[$1]=gptr;
                           NETLIST.AddGate(gptr);
                       }
                       else {
						   gptr=(*KaiNameTable.find($1)).second;
                       }
                       gptr->SetFunction($3);
					   if(gptr->GetFunction()==G_NOT
						  ||gptr->GetFunction()==G_NOR
						  ||gptr->GetFunction()==G_NAND) {
						   gptr->SetInverse(true);
					   }
					   else {
						   gptr->SetInverse(false);
					   }

                 } fanin_list RPAR EOLINE
;

gatename: GNOT { $$=G_NOT; }
         |GAND { $$=G_AND; }
         |GOR { $$=G_OR; }
         |GNAND { $$=G_NAND; }
         |GNOR { $$=G_NOR; }
		 |GBUF { $$=G_BUF; }
		 |GXOR { $$=G_XOR; }
		 |GDFF { $$=G_DFF; }
;

fanin_list: 
          NAMESTRING {
			  if(KaiNameTable.find($1)==KaiNameTable.end()) {
                  inlist=new KaiGATE;
                  inlist->SetName($1);
				  KaiNameTable[$1]=inlist;
                  NETLIST.AddGate(inlist);
                  gptr->AddFanin(inlist);
              }
              else {
				  inlist=(*KaiNameTable.find($1)).second;
                  gptr->AddFanin(inlist);
              }
          } 
          | NAMESTRING {
			  if(KaiNameTable.find($1)==KaiNameTable.end()) {
                  inlist=new KaiGATE;
                  inlist->SetName($1);
				  KaiNameTable[$1]=inlist;
                  NETLIST.AddGate(inlist);
                  gptr->AddFanin(inlist);
              }
              else {
				  inlist=(*KaiNameTable.find($1)).second;
                  gptr->AddFanin(inlist);
              }
          } COMMA fanin_list 
;

%%

int kkerror(char *err)
{
   	cerr<<err<<endl;
   	cerr<<"\t Parsing error in line "<<kai_lineno<<" at "<<kktext<<endl;
   	ParseError=true;
   	return -1;
}
