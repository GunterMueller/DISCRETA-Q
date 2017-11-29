#ifndef __GEOMETRY_H
#define __GEOMETRY_H

// Michael Braun
// 2003.01.15
// geometry.h

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/*#############################################################################
##                                                                           ##
##                         PROJECTIVE GEOMETRY                               ##
##                                                                           ##
#############################################################################*/


/*****************************************************************************/
void first_PG_element(
        int n,           // natural number n
        int q,           // prime power q
        vector<int>& x   // return: vector x with the first element
);                       // of PG(n-1,q), x := [1,0,...0]


/*****************************************************************************/
void next_PG_element(
       int n,            // natural number n
       int q,            // prime power q
       vector<int>& x    // return: overwrites x with the next element
);                       // of PG(n-1,q)


/*****************************************************************************/
void last_PG_element(
        int n,           // natural number n
        int q,           // prime power q
        vector<int>& x   // return: vector x with the last element
);                       // of PG(n-1,q), x := [1,q-1,...q-1]


/*****************************************************************************/
unsigned int PG_order(
        int n,           // natural number n
        int q            // prime power q
);                       // return: the number of points in PG(n-1,q)


/*****************************************************************************/
unsigned int rank_PG_element(
        int n,                 // natural number n
        int q,                 // prime power q
        const vector<int>& x   // element x of PG(n-1,q)
);                             // return: a natural number


/*#############################################################################
##                                                                           ##
##                      VECTOR SPACE V = GF(q)^n                             ##
##                                                                           ##
#############################################################################*/


/*****************************************************************************/
void first_V_element(
        int n,           // natural number n
        int q,           // prime power q
        vector<int>& x   // return: vector x with the first element
                         // of V = GF(q)^n,  x = [0,...,0]
);


/*****************************************************************************/
void next_V_element(
        int n,          // natural number n
        int q,          // prime power q
        vector<int>& x  // return: overwrites x with the next element
);                      // of V = GF(q)^n


/*****************************************************************************/
void last_V_element(
        int n,           // natural number n
        int q,           // prime power q
        vector<int>& x   // return: vector x with the last element
);                       // of V = GF(q)^n,  x = [q-1,...,q-1]


/*****************************************************************************/
unsigned int rank_V(
        int n,                // natural number n
        int q,                // prime power q
        const vector<int>& x  // element x of V = GF(q)^n
);                            // return:


#endif

