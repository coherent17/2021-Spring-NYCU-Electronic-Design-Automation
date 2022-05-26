
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

#ifndef CSAT_UTIL_H
#define CSAT_UTIL_H
using namespace std;

#include <assert.h>     // for GNU compilier, for others --> string.h
#include <string>     // for GNU compilier, for others --> string.h
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>

// currently, char* string is not supported, you can chose string
// or extend hash definition by adding CMP_FUNC in the hash template
// definition, and modify the member functions accordingly.
extern long current_cpu_time();

struct S_strhash
{
int operator() (const string id, const int size)
    {
    int val = 0;
    int c = 0;
    for (unsigned i = 0; i < id.size(); i++)
	{
	c = id[i];
	val = val*997 + c;
	}
    return abs(val%size);
    }
};

template<class KEY>
struct S_ptrhash
{
int operator() (const KEY id, const int size)
    {
    return ((int)((long)(id)>>2)%size);
    }
};

struct S_numhash
{
int operator() (const int id, const int size)
    {
    return (abs((int)(id)%(size)));
    }
};

template<class KEY, class VALUE, class HASH_FUNC>
class hash
{
int _num_bins;
int _num_entries;
int _max_density;
int _reorder_flag;
double _grow_factor;
vector<list<pair<KEY,VALUE> > > _bin;

public:

hash(int size = 503, int density = 5, double grow_factor = 2.0,
	int reorder_flag = 0)
    {
    _num_entries = 0;
    _max_density = density;
    _grow_factor = grow_factor;
    _reorder_flag = reorder_flag;
    if (size <= 0)
	size = 503;
    _num_bins = size;
    _bin.resize(_num_bins);
    };
~hash()
    {
    clear();
    _bin.clear();
    }
void clear();
bool rehash();
bool lookup(KEY key, VALUE& value);
bool insert(KEY key, VALUE value);		
bool remove(KEY key);

void dump(ostream& os)
    {
    os<<"size = "<<_num_bins<<"density = "<<_max_density<<endl;
    }

VALUE& operator[] (KEY k);
VALUE  operator() (KEY k);
};

// clear the hash table
template<class KEY, class VALUE, class HASH_FUNC>
void 
hash<KEY, VALUE, HASH_FUNC>::clear()
{
for (int i = 0; i < _num_bins; i++)
    {
    _bin[i].clear();
    }
_num_entries = 0;
}


template<class KEY, class VALUE, class HASH_FUNC>
bool 
hash<KEY, VALUE, HASH_FUNC>::lookup(KEY key, VALUE& value)
{
int hash_val;
HASH_FUNC hf;

hash_val = hf(key,_num_bins);
if (_bin[hash_val].empty())
    return false;
typename list<pair<KEY, VALUE> >::iterator li = _bin[hash_val].begin();
typename list<pair<KEY, VALUE> >::iterator last = _bin[hash_val].end();
for (; li != last; li++)
    {
    if ((*li).first == key)
	{
	//cout<<" look up key = "<<key<<" find value = "<<(*li).second<<endl;
	value = (*li).second;
	return true;
	}
    }
//cout<<" look up key = "<<key<<" not find"<<endl;
return 0;
}

#define ADD_DIRECT(key, value, hash_val) \
{\
if (_num_entries/_num_bins >= _max_density) \
    {\
    rehash();\
    hash_val = hf(key,_num_bins);\
    }\
\
_bin[hash_val].push_back(make_pair(key, value));\
_num_entries++;\
}	

// when the key already exists return 0 and 
template<class KEY, class VALUE, class HASH_FUNC>
bool 
hash<KEY, VALUE, HASH_FUNC>::insert(KEY key, VALUE value)
{
int hash_val;
HASH_FUNC hf;

hash_val = hf(key,_num_bins);
if (_bin[hash_val].empty())
    {
    ADD_DIRECT(key, value, hash_val);		
    //cout<<"key = "<<key<<" insert in "<<hash_val<<endl;
    return 0;
    }
typename list<pair<KEY, VALUE> >::iterator li = _bin[hash_val].begin();
typename list<pair<KEY, VALUE> >::iterator last = _bin[hash_val].end();
for (; li != last; li++)
    {
    if ((*li).first == key)
	{
	//cout<<"key exist "<<key<<" old value "<<(*li).second<<" new "<<value;	
	//cout<<endl;
	(*li).second = value;
	return 1;
	}
    }
ADD_DIRECT(key, value, hash_val);
//cout<<"key = "<<key<<" insert in "<<hash_val<<endl;
return 0;
}


template<class KEY, class VALUE, class HASH_FUNC>
bool 
hash<KEY, VALUE, HASH_FUNC>::rehash()
{
int hash_val;
HASH_FUNC hf;

vector<list<pair<KEY,VALUE> > > new_bin;
int old_num_bins = _num_bins;
int old_num_entries = _num_entries;
_num_bins = (int)(_grow_factor * old_num_bins);
if (_num_bins % 2 == 0)
    _num_bins++;
assert(_num_bins > old_num_bins);

//cout<<"rehash newbinnum "<<_num_bins<< " oldbinnum = "<<old_num_bins<<endl;

_num_entries = 0;
/*
new_bin.reserve(_num_bins);
for (int i = 0; i < _num_bins; i++)
    {
    list<pair<KEY, VALUE> > lst;
    new_bin.push_back(lst);
    }
*/
new_bin.resize(_num_bins);
for (int i = 0; i < old_num_bins; i++)
    {
    typename list<pair<KEY, VALUE> >::iterator li = _bin[i].begin();
    typename list<pair<KEY, VALUE> >::iterator last = _bin[i].end();
    for (; li != last; li++)
	{
	hash_val = hf((*li).first,_num_bins);
	new_bin[hash_val].push_back(*li);
//	cout<<"key = "<<(*li).first<<" original = "<<i;
//	cout<<" new = "<<hash_val<<endl;
	_num_entries++;
	}
    _bin[i].clear();
    }
assert(_num_entries == old_num_entries);
_bin.clear();
_bin = new_bin;
return 1;
}

template<class KEY, class VALUE, class HASH_FUNC>
bool 
hash<KEY, VALUE, HASH_FUNC>::remove(KEY key)
{
int hash_val;
HASH_FUNC hf;

hash_val = hf(key,_num_bins);
if (_bin[hash_val].empty())
    return 0;
typename list<pair<KEY, VALUE> >::iterator li = _bin[hash_val].begin();
typename list<pair<KEY, VALUE> >::iterator last = _bin[hash_val].end();
for (; li != last; li++)
    {
    if ((*li).first == key)
	{
//	cout<<" remove = "<<key<<" value = "<<(*li).second<<endl;
	_bin[hash_val].erase(li);
	return 1;
	}
    }
return 0;
}


template<class KEY, class VALUE, class HASH_FUNC>
VALUE& 
hash<KEY, VALUE, HASH_FUNC>::operator[](KEY key)
{
int hash_val;
HASH_FUNC hf;
VALUE value(0);

hash_val = hf(key,_num_bins);
if (_bin[hash_val].empty())
    {
    ADD_DIRECT(key, value, hash_val);		
    //cout<<"key = "<<key<<" will be insert in "<<hash_val<<endl;
    return _bin[hash_val].back().second;
    }
typename list<pair<KEY, VALUE> >::iterator li = _bin[hash_val].begin();
typename list<pair<KEY, VALUE> >::iterator last = _bin[hash_val].end();
for (; li != last; li++)
    {
    if ((*li).first == key)
	{
//	cout<<"key exist "<<key<<" old value "<<(*li).second;	
//	cout<<endl;
	return (*li).second;
	}
    }
ADD_DIRECT(key, value, hash_val);
//cout<<"key = "<<key<<" will insert in "<<hash_val<<endl;
return _bin[hash_val].back().second;
}

template<class KEY, class VALUE, class HASH_FUNC>
VALUE 
hash<KEY, VALUE, HASH_FUNC>::operator()(KEY key)
{
int hash_val;
HASH_FUNC hf;
VALUE value(0);

hash_val = hf(key,_num_bins);
if (_bin[hash_val].empty())
    {
    //cout<<"no find key = "<<key<<endl;
    return value;
    }
typename list<pair<KEY, VALUE> >::iterator li = _bin[hash_val].begin();
typename list<pair<KEY, VALUE> >::iterator last = _bin[hash_val].end();
for (; li != last; li++)
    {
    if ((*li).first == key)
	{
//	cout<<"find "<<key<<"value "<<(*li).second;	
//	cout<<endl;
	return (*li).second;
	}
    }
//cout<<"no find key = "<<key<<endl;
return value;
};

class CTime
{
private:
long _starttime;
public:
CTime()
    {
    _starttime = current_cpu_time();
    }
void init()
    {
    _starttime = current_cpu_time();
    }
long get_time()
    {
    return _starttime;
    }
void run_time(char* name)
    {
    double runtime = 1.0*(current_cpu_time() -_starttime)/1000;
    _starttime = current_cpu_time();
    cout<<name <<" run time " <<runtime<<endl;
    }
};

#endif
