#include <stdio.h>
#include <stdlib.h>
#include "cudd.h"
#include "util.h"
#include "config.h"

#include "readfile.h"
#include "buildBDD.h"

int main(int argc, char *argv[]){
	char *inputName = *(argv + 1);
	//char *outputName = *(argv + 2);

	RawData rawdata;
	
	//read file
	readFile(inputName, &rawdata);
	printRawData(rawdata);
	return 0;
}