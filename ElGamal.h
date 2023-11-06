//
//  ElGamal.h
//  DEAL_and_ELGAMAL
//
//  Created by Murad Daudov on 30.10.2023.
//



#ifndef ElGamal_h
#define ElGamal_h

#include <iostream>
#include "largeClass.h"
#include "stdlib.h"
#include "time.h"
#include <string>
#include <fstream>
#include "DEAL.hpp"

using namespace std;

void encrypt(largeClass m, largeClass p, largeClass g, largeClass y, largeClass& a, largeClass& b)
{
    largeClass pp = p - 2;
    largeClass k = GenRandomByRangeC(pp) + 1;
    a = powModClass(g, k, p);
    b = powModClass(y, k, p);
    b = (b * m) % p;
}

largeClass decrypt(largeClass p, largeClass x, largeClass a, largeClass b)
{
    largeClass tmp = p - 1 - x;
    return (b * powModClass(a, tmp, p)) % p;
}

#endif /* ElGamal_h */
