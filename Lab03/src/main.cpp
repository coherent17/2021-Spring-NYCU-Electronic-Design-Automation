#include <stdio.h>
#include <stdlib.h>
#include "cudd.h"
#include "util.h"
#include "config.h"

#include "../src/lib/readfile.h"


int main(int argc, char *argv[]){
	char *inputName = *(argv + 1);
	//char *outputName = *(argv + 2);

	RawData rawdata;
	readFile(inputName, &rawdata);
	printRawData(rawdata);
	
	return 0;
}