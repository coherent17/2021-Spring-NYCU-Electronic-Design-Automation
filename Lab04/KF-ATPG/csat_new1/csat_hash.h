
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

#ifndef CSAT_HASH_H
#define CSAT_HASH_H
using namespace std;

#include <assert.h>   
#include <string>   
#include <vector>
#include <list>
#include <set>
#include <algorithm>
#include <iostream>
#include <fstream>

// currently, char* string is not supported, you can chose string
// or extend hash definition by adding CMP_FUNC in the hash template
// definition, and modify the member functions accordingly.

template<class KEY, class VALUE, class HASH_FUNC,class CPR_FUNC>
class hash_cpr
{
int _num_bins;
int _num_entries;
int _max_density;
int _reorder_flag;
double _grow_factor;
vector<list<pair<KEY,VALUE> > > _bin;

public:

hash_cpr(int size = 503, int density = 5, double grow_factor = 2.0,
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
~hash_cpr()
    {
    clear();
    _bin.clear();
    }
void clear();
bool rehash();
bool lookup(KEY key, VALUE& value);
bool insert(KEY key, VALUE value);		
bool insert_noupdate(KEY key, VALUE value);		
bool remove(KEY key);

void dump(ostream& os)
    {
    os<<"size = "<<_num_bins<<"density = "<<_max_density<<endl;
    }

VALUE& operator[] (KEY k);
VALUE  operator() (KEY k);
};

// clear the hash table
template<class KEY, class VALUE, class HASH_FUNC, class CPR_FUNC>
void 
hash_cpr<KEY, VALUE, HASH_FUNC, CPR_FUNC>::clear()
{
for (int i = 0; i < _num_bins; i++)
    {
    _bin[i].clear();
    }
_num_entries = 0;
}


template<class KEY, class VALUE, class HASH_FUNC, class CPR_FUNC>
bool 
hash_cpr<KEY, VALUE, HASH_FUNC, CPR_FUNC>::lookup(KEY key, VALUE& value)
{
int hash_val;
HASH_FUNC hf;
CPR_FUNC cpr;

hash_val = hf(key,_num_bins);
if (_bin[hash_val].empty())
	return false;
typename list<pair<KEY, VALUE> >::iterator li = _bin[hash_val].begin();
typename list<pair<KEY, VALUE> >::iterator last = _bin[hash_val].end();
for (; li != last; li++)
    {
    if (cpr.equal((*li).first, key))
	{
	value = (*li).second;
	return true;
	}
    }
return false;
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
template<class KEY, class VALUE, class HASH_FUNC, class CPR_FUNC>
bool 
hash_cpr<KEY, VALUE, HASH_FUNC, CPR_FUNC>::insert(KEY key, VALUE value)
{
int hash_val;
HASH_FUNC hf;
CPR_FUNC cpr;

hash_val = hf(key,_num_bins);
if (_bin[hash_val].empty())
    {
    ADD_DIRECT(key, value, hash_val);		
    //cout<<"key = "<<key<<" insert in "<<hash_val<<endl;
    return 1;
    }
typename list<pair<KEY, VALUE> >::iterator li = _bin[hash_val].begin();
typename list<pair<KEY, VALUE> >::iterator last = _bin[hash_val].end();
for (; li != last; li++)
    {
    if (cpr.equal((*li).first, key))
	{
	(*li).second = value;
	return 0;
	}
    }
ADD_DIRECT(key, value, hash_val);
return 1;
}


template<class KEY, class VALUE, class HASH_FUNC, class CPR_FUNC>
bool 
hash_cpr<KEY, VALUE, HASH_FUNC, CPR_FUNC>::insert_noupdate(KEY key, VALUE value)
{
int hash_val;
HASH_FUNC hf;
CPR_FUNC cpr;

hash_val = hf(key,_num_bins);
if (_bin[hash_val].empty())
    {
    ADD_DIRECT(key, value, hash_val);		
    //cout<<"key = "<<key<<" insert in "<<hash_val<<endl;
    return 1;
    }
typename list<pair<KEY, VALUE> >::iterator li = _bin[hash_val].begin();
typename list<pair<KEY, VALUE> >::iterator last = _bin[hash_val].end();
for (; li != last; li++)
    {
    if (cpr.equal((*li).first, key))
	{
	return 0;
	}
    }
ADD_DIRECT(key, value, hash_val);
return 1;
}

template<class KEY, class VALUE, class HASH_FUNC, class CPR_FUNC>
bool 
hash_cpr<KEY, VALUE, HASH_FUNC, CPR_FUNC>::rehash()
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

_num_entries = 0;
new_bin.resize(_num_bins);
for (int i = 0; i < old_num_bins; i++)
    {
    typename list<pair<KEY, VALUE> >::iterator li = _bin[i].begin();
    typename list<pair<KEY, VALUE> >::iterator last = _bin[i].end();
    for (; li != last; li++)
	{
	hash_val = hf((*li).first,_num_bins);
	new_bin[hash_val].push_back(*li);
	_num_entries++;
	}
    _bin[i].clear();
    }
assert(_num_entries == old_num_entries);
_bin.clear();
_bin = new_bin;
return 1;
}

template<class KEY, class VALUE, class HASH_FUNC, class CPR_FUNC>
bool 
hash_cpr<KEY, VALUE, HASH_FUNC, CPR_FUNC>::remove(KEY key)
{
int hash_val;
HASH_FUNC hf;
CPR_FUNC cpr;

hash_val = hf(key,_num_bins);
if (_bin[hash_val].empty())
    return 0;
typename list<pair<KEY, VALUE> >::iterator li = _bin[hash_val].begin();
typename list<pair<KEY, VALUE> >::iterator last = _bin[hash_val].end();
for (; li != last; li++)
    {
    if (cpr.equal((*li).first, key))
	{
	_bin[hash_val].erase(li);
	return 1;
	}
    }
return 0;
}


template<class KEY, class VALUE, class HASH_FUNC, class CPR_FUNC>
VALUE& 
hash_cpr<KEY, VALUE, HASH_FUNC, CPR_FUNC>::operator[](KEY key)
{
int hash_val;
HASH_FUNC hf;
CPR_FUNC cpr;
VALUE value(0);

hash_val = hf(key,_num_bins);
if (_bin[hash_val].empty())
    {
    ADD_DIRECT(key, value, hash_val);		
    return _bin[hash_val].back().second;
    }
typename list<pair<KEY, VALUE> >::iterator li = _bin[hash_val].begin();
typename list<pair<KEY, VALUE> >::iterator last = _bin[hash_val].end();
for (; li != last; li++)
    {
    if (cpr.equal((*li).first,key))
	{
	return (*li).second;
	}
    }
ADD_DIRECT(key, value, hash_val);
return _bin[hash_val].back().second;
}

template<class KEY, class VALUE, class HASH_FUNC, class CPR_FUNC>
VALUE 
hash_cpr<KEY, VALUE, HASH_FUNC,CPR_FUNC>::operator()(KEY key)
{
int hash_val;
HASH_FUNC hf;
CPR_FUNC cpr;
VALUE value(0);

hash_val = hf(key,_num_bins);
if (_bin[hash_val].empty())
    {
//  cout<<"no find key = "<<key<<endl;
    return value;
    }
typename list<pair<KEY, VALUE> >::iterator li = _bin[hash_val].begin();
typename list<pair<KEY, VALUE> >::iterator last = _bin[hash_val].end();
for (; li != last; li++)
    {
    if (cpr.equal((*li).first,key))
	{
	return (*li).second;
	}
    }
return value;
};

#endif
