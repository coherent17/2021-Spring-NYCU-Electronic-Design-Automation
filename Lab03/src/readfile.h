#ifndef _READFILE_H_
#define _READFILE_H_

#include <vector>
#include <map>
using namespace std;
using std::vector;
using std::map;

#define BUFFER_SIZE 10000

typedef struct _Variable{
	int ASCII_id;			//with base 'a' or 'A'
	bool isNegative;
}Variable;

typedef struct _RawData{
	//ex: Expression = ab + cd, then ab store in vector[0], cd store in vector[1]
	vector <vector<Variable>> Literals;
	vector <vector<int>> VarOrder;
	vector<map<int, int>> VarIndex;
} RawData;

void readFile(char *filename, RawData *rawdata);
void printRawData(RawData rawdata);

#endif