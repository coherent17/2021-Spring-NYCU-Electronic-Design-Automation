
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

#ifndef CSAT_STRUCTURE_H
#define CSAT_STRUCTURE_H

#include "csat_gate.h"

struct S_pand2hash
{
int operator() (CAnd* pand, const int size)
    {
    assert(pand->vin().size() == 2);
    return abs(((pand->vin()[0] | pand->vin()[1]) + pand->vin()[0] + 
        pand->vin()[1] + (pand->vin()[0] & pand->vin()[1])))%size;
    }
};

struct S_pand2cpr
{
bool equal(CAnd* g1, CAnd* g2)
    {
    int sin1;
    int bin1;
    int sin2;
    int bin2;
    if (g1->vin()[0] < g1->vin()[1])
        {
        sin1 = g1->vin()[0];
        bin1 = g1->vin()[1];
        }
    else
        {
        sin1 = g1->vin()[1];
        bin1 = g1->vin()[0];
        }
    if (g2->vin()[0] < g2->vin()[1])
        {
        sin2 = g2->vin()[0];
        bin2 = g2->vin()[1];
        }
    else
        {
        sin2 = g2->vin()[1];
        bin2 = g2->vin()[0];
        }
    if ((sin1 == sin2) && (bin1 == bin2))
        return true;
    else
        return false;
    }
};

struct S_pandhash
{
int operator() (CAnd* pand, const int size)
    {
    int result = 0;
    int tmpand = ~0;
    int tmpor = 0;
    result += pand->vin().size();
    for (int i = 0; i < pand->vin().size();i++)
        {
        tmpand = tmpand & pand->vin()[i];
        tmpor = tmpor | pand->vin()[i];
        result += pand->vin()[i];        
	} 
    result += tmpand + tmpor;
    return abs(result%size);
    }
};

struct S_pandfaninhash
{
int operator() (CAnd* pand, const int size)
    {
    int result = 0;
    int tmpand = ~0;
    int tmpor = 0;
    result += pand->vfanin().size();
    for (int i = 0; i < pand->vfanin().size();i++)
        {
        tmpand = tmpand & pand->vfanin()[i];
        tmpor = tmpor | pand->vfanin()[i];
        result += pand->vfanin()[i];        
	} 
    result += tmpand + tmpor;
    return abs(result%size);
    }
};
struct S_pandcpr
{
bool equal(CAnd* g1, CAnd* g2)
    {
    if (g1->vin().size() != g2->vin().size())
        return false;
    for (int i = 0; i < g1->vin().size();i++)
        {
        if (g1->vin()[i] != g2->vin()[i])
            return false;
        }
    return true;
    }    
};

struct S_pandfanincpr
{
bool equal(CAnd* g1, CAnd* g2)
    {
    if (g1->vfanin().size() != g2->vfanin().size())
        return false;
    for (int i = 0; i < g1->vfanin().size();i++)
        {
        if (g1->vfanin()[i] != g2->vfanin()[i])
            return false;
        }
    return true;
    }    
};

struct S_pxorhash
{
int operator() (CAnd* pand, const int size)
    {
    assert(pand->opgate() != NULL);
    SOPGate* opg = pand->opgate();
    assert(opg->_type == CSAT_BXOR);
    int id0 = SIGID(opg->_vin[0]);
    int id1 = SIGID(opg->_vin[1]);

    return abs(((id0 | id1) + id0 + id1
        + (id0 & id1)))%size;
    }
};

struct S_pxorcpr
{
bool equal(CAnd* g1, CAnd* g2)
    {
    assert(g1->opgate() != NULL);
    assert(g2->opgate() != NULL);

    int id00 = SIGID(g1->opgate()->_vin[0]);
    int id01 = SIGID(g1->opgate()->_vin[1]);
    int id10 = SIGID(g2->opgate()->_vin[0]);
    int id11 = SIGID(g2->opgate()->_vin[1]);
    if ((id00 == id10) && (id01 == id11))
	return true;
    return false;
    }
};

#endif
