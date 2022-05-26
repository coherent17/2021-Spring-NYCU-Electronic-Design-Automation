
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

#ifndef CSAT_MACRO_H
#define CSAT_MACRO_H

// for packing signal id and sign in an 32-bit integer
#define SIGN(i) (i& 0x1)
#define VALUE(i) (!(i& 0x1))
#define SIGVAL(i) (!(i & 0x1))
#define RABS(i) (i >= 0? i<<1:((abs(i)<<1)|0x1))
#define SIGID(i) (i>>1)
#define PACKID_SIGN(i,sign) ((i<<1)|(sign)) 
#define PACKID_PHASE(i,phase) ((i<<1)|(!phase)) 
#define PACKID_VALUE(i,value) ((i<<1)|(!value)) 
#define SIGNID(i) (SIGN(i)?-SIGID(i):SIGID(i))

// for conflict gate

#define NEW(CLASS_NAME, MANAGER_NAME) \
inline void* \
CLASS_NAME::operator new(size_t size) \
{ \
assert(size == sizeof(CLASS_NAME)); \
if (MANAGER_NAME._freelist != NULL) \
    { \
    void* ptr = MANAGER_NAME._freelist; \
    MANAGER_NAME._freelist = MANAGER_NAME._freelist->_next; \
    return ptr; \
    } \
char* ptr = new char[size * MANAGER_NAME._block_size]; \
if (ptr == NULL) \
    return ptr; \
CLASS_NAME* pobject = (CLASS_NAME*)ptr; \
MANAGER_NAME._vblock.push_back(ptr); \
for (int i = 0; i < MANAGER_NAME._block_size-1;i++) \
    { \
    SFree* fptr = (SFree*)pobject; \
    fptr->_next = MANAGER_NAME._freelist; \
    MANAGER_NAME._freelist = fptr;\
    pobject++;\
    } \
return (void*)pobject;\
}

#define DELETE(CLASS_NAME, MANAGER_NAME) \
inline void \
CLASS_NAME::operator delete(void* ptr) \
{ \
SFree* fptr = (SFree*)(ptr); \
SFree* ptr1 = MANAGER_NAME._freelist; \
MANAGER_NAME._freelist = fptr;\
MANAGER_NAME._freelist->_next = ptr1;\
}


#define ADD_COMMENT(COMMENT) \
static int _call_time = 0; \
if (_call_time == 0)       \
    _comment += COMMENT;   \
_call_time++;

#endif
