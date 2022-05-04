#ifndef _READFILE_H_
#define _READFILE_H_

#include <vector>
using namespace std;
using std::vector;

#define BUFFER_SIZE 10000

typedef struct _RawData{
	char expression[BUFFER_SIZE];
	vector <vector<int>> VarOrder;
} RawData;

void readFile(char *filename, RawData *rawdata);
void printRawData(RawData rawdata);

#endif