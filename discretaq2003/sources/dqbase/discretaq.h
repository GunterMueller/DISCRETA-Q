/*#############################################################################
##                                                                           ##
##   #####   #   ####    ####   #####   ######  #####    ##          ####    ##
##   #    #  #  #       #    #  #    #  #         #     #  #        #    #   ##
##   #    #  #   ####   #       #    #  #####     #    #    #  ###  #    #   ##
##   #    #  #       #  #       #####   #         #    ######       #  # #   ##
##   #    #  #  #    #  #    #  #   #   #         #    #    #       #   ##   ##
##   #####   #   ####    ####   #    #  ######    #    #    #        #####   ##
##                                                                           ##
#############################################################################*/

// Michael Braun
// 2003.01.15
// discretaq.h

/*#############################################################################
##                                                                           ##
##                                INCLUDES                                   ##
##                                                                           ##
#############################################################################*/

#include "polynomial.h"
#include "matrix.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstring>

using namespace std;

/*#############################################################################
##                                                                           ##
##                         INTEGER COMBINATORICS                             ##
##                                                                           ##
#############################################################################*/

/*****************************************************************************/
vector<int> prime_factorization(
        int n                    // natural number n
)                                // vector of natural numbers which
                                 // are the prime factors of n
/*****************************************************************************/
{
  if (n < 1)
  {
    cout << "ERROR: is_prime(n): n must be greater than 0!\n";
    exit(1);
  }
  int number;
  bool factoring = true;
  vector<int> v;
  v.assign(0,0);
  number = n;
  if (n == 1) v.push_back(n);
  else
    while(factoring)
      for (int i = 2; i <= number; i++)
      {
        if (i == number) factoring = false;
        if (number % i == 0)
        {
          v.push_back(i);
          number =  number / i;
          break;
        }
      }
  return v;
}


/*****************************************************************************/
bool is_prime(
        int n        // number n
)                    // return: true if n is prime and false else
/*****************************************************************************/
{
  vector<int> v;
  v = prime_factorization(n);
  return (v.size() == 1) ? true : false;
}


/*****************************************************************************/
bool is_prime_power(
        int n        // natural number n
)                    // return: true if n is prime_power and false else
/*****************************************************************************/
{
  vector<int> v;
  v = prime_factorization(n);
  return (v[0] == v[v.size()-1]) ? true : false;
}


/*****************************************************************************/
unsigned long int power_n(
            int x,             // natural number x
            int n              // natural number n
    )                          // return: x^n
/*****************************************************************************/
{
  return ((unsigned long int)pow((double)x,n));
}


/*****************************************************************************/
unsigned long int gauss_nkq(
        int n,               // natural number n
        int k,               // natural number k
        int q                // prime power q
)                            // return: number of k-subspaces of the
                             // vector space GF(q)^n
/*****************************************************************************/
{
  if (n < 0)
  {
    cout << "ERROR: gauss_nkq(n,k,q): n must be greater equal than 0!\n";
    exit(1);
  }
  if (k < 0)
  {
    cout << "ERROR: gauss_nkq(n,k,q): k must be greater equal than 0!\n";
    exit(1);
  }
  if (!is_prime_power(q))
  {
    cout << "ERROR: gauss_nkq(n,k,q): q is not a prime power!\n";
    exit(1);
  }
  if (k > n)
    return 0;
  else
  {
    matrix<unsigned long int> A(n+1,n+1);
    A[0][0] = 1;
    for (int i = 1; i <= n; i++)
    {
      A[i][0] = 1;
      for (int j = 1; j <= i-1; j++)
        A[i][j] = A[i-1][j-1] + power_n(q,j) * A[i-1][j];
      A[i][i] = 1;
    }
    return A[n][k];
  }
}


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
)                        // of PG(n-1,q), x := [1,0,...0]
/*****************************************************************************/
{
  x.assign(n,0);
  x[n-1] = 1;
}


/*****************************************************************************/
void next_PG_element(
       int n,            // natural number n
       int q,            // prime power q
       vector<int>& x    // return: overwrites x with the next element
)                        // of PG(n-1,q)
/*****************************************************************************/
{
  int i, pos = n-1, last_pos;
  for (i = 0; i < n; i++) if (x[i] == 1) break;
  last_pos = i;
  if (last_pos == n-1)
  {
    x[n-1] = 0;
    x[n-2] = 1;
  }
  else
  {
    while (pos > last_pos)
    {
      if (x[pos] < q-1)
      {
        x[pos]++;
        break;
      }
      else
      {
        x[pos] = 0;
        pos--;
      }
      if (pos == last_pos)
      {
        x[last_pos] = 0;
        x[last_pos-1] = 1;
      }
    }
  }
}


/*****************************************************************************/
void last_PG_element(
        int n,           // natural number n
        int q,           // prime power q
        vector<int>& x   // return: vector x with the last element
)                        // of PG(n-1,q), x := [1,q-1,...q-1]
/*****************************************************************************/
{
  x.assign(n,q-1);
  x[0] = 1;
}


/*****************************************************************************/
unsigned long int  PG_order(
        int n,           // natural number n
        int q            // prime power q
)                        // return: the number of points in PG(n-1,q)
/*****************************************************************************/
{
  return (power_n(q,n)-1) / (q-1);
}


/*****************************************************************************/
unsigned long int rank_PG_element(
        int n,                 // natural number n
        int q,                 // prime power q
        const vector<int>& x   // element x of PG(n-1,q)
)                              // return: a natural number
/*****************************************************************************/
{
  unsigned long int r;
  int i, pos = n-1, last_pos;
  for (i = 0; i < n; i++)
    if (x[i] == 1) break;
  last_pos = i;
  r = PG_order(n-last_pos-1,q);
  for (i = 0; i < n-last_pos-1; i++)
    r += x[n-1-i] * power_n(q,i);
  return r;
}


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
)                        // of V = GF(q)^n,  x = [0,...,0]
/*****************************************************************************/
{
  x.assign(n,0);
}


/*****************************************************************************/
void next_V_element(
        int n,          // natural number n
        int q,          // prime power q
        vector<int>& x  // return: overwrites x with the next element
)                       // of V = GF(q)^n
/*****************************************************************************/
{
  int pos = 0;
  while( pos < n )
  {
    if ( x[pos] < q-1 )
    {
      x[pos]++;
      break;
    }
    else
    {
      x[pos] = 0;
      pos++;
    }
  }
}


/*****************************************************************************/
void last_V_element(
        int n,           // natural number n
        int q,           // prime power q
        vector<int>& x   // return: vector x with the last element
)                        // of V = GF(q)^n,  x = [q-1,...,q-1]
/*****************************************************************************/
{
  x.assign(n,q-1);
}


/*****************************************************************************/
unsigned long int rank_V(
        int n,                // natural number n
        int q,                // prime power q
        const vector<int>& x  // element x of V = GF(q)^n
)                             // return:
/*****************************************************************************/
{
  unsigned long int tmp = 0;
  for (int i = 0; i < n; i++)
    tmp += x[i] * power_n(q,i);
  return tmp;
}


/*#############################################################################
##                                                                           ##
##                      FINITE FIELD GF(q) with q elements                   ##
##                                                                           ##
#############################################################################*/


static unsigned int q;       // field with q elements


static matrix<unsigned int>
        Add_table,           // addition table of GF(q)
        Mult_table;          // multiplication table of GF(q)


static vector<unsigned int>
        Add_invert_table,    // invers elements resp. addition
        Mult_invert_table;   // invers elements resp. multiplication


unsigned int field() { return q; }  // returns order of the field


/*****************************************************************************/
void print_tables() // print * and + table of the field with q elements
/*****************************************************************************/
{
  cout << "tables for the field with " << field() << " elements\n\n";
  cout << "* table";
  cout << Mult_table << endl;
  cout << "+ table";
  cout << Add_table << endl;
}


class gfq
{
  protected:


    short element; // element of GF(q)


  public:


/*****************************************************************************/
    gfq()   // Standardconstructor
/*****************************************************************************/
    {
      element = 0;
    }


/*****************************************************************************/
    gfq(                       // constructor
           unsigned int x      // element x of GF(q)
    )                          // return: this = x
/*****************************************************************************/
    {
      element = x % q;
    }


/*****************************************************************************/
    gfq& init(
            unsigned int x     // element x of GF(q)
    )                          // return: this = x
/*****************************************************************************/
    {
      element = x % q;
      return *this;
    }


/*****************************************************************************/
    unsigned int value() const  // return: value of this, i.e. element
/*****************************************************************************/
    {
      return element;
    }


/*****************************************************************************/
    bool operator==(
           const gfq& x    // element x of GF(q)
    ) const                // return: true if x is this, else false
/*****************************************************************************/
    {
      return value() == x.value();
    }


/*****************************************************************************/
    bool operator!=(
           const gfq& x    // element x of GF(q)
    ) const                // return: true if x is this, else false
/*****************************************************************************/
    {
      return value() != x.value();
    }


/*****************************************************************************/
    bool operator<(
            const gfq& x    // element x of GF(q)
    ) const                 // return: true if x is less tha this, else false
/*****************************************************************************/
    {
      return value() < x.value();
    }


/*****************************************************************************/
    gfq& power_n(
            unsigned long n   // natural number n
    )                         // return: this = this ^ n
/*****************************************************************************/
    {
      gfq v = (*this), zero(0);
      unsigned long i = n-1;
      if ((*this) != zero)
      {
        while (i > 0)
        {
          if ( (i%2) == 0 )
          {
            v *= v;
            i >>= 1;
          }
          (*this) *= v;
          i--;
        }
      }
      return *this;
    }


/*****************************************************************************/
    gfq& invert()   // return: this = this^{-1}
/*****************************************************************************/
    {
      int tmp = value();
      init(Mult_invert_table[tmp]);
      return (*this);
    }


/*****************************************************************************/
    gfq operator+(
            const gfq& x   // gfq object x
    ) const                // return: this + x
/*****************************************************************************/
    {
      gfq tmp(Add_table[value()][x.value()]);
      return tmp;
    }


/*****************************************************************************/
    gfq& operator+=(
            const gfq& x   // gfq object x
    )                      // return: this = this + x
/*****************************************************************************/
    {
      (*this) = (*this) + x;
      return *this;
    }


/*****************************************************************************/
    gfq operator-()   // return: - this
/*****************************************************************************/
    {
      gfq tmp;
      tmp.init(Add_invert_table[value()]);
      return tmp;
    }


/*****************************************************************************/
    gfq operator-(
            const gfq& x   // gfq object x
    ) const                // return: this - x
/*****************************************************************************/
    {
      gfq x_ai(Add_invert_table[x.value()]);
      gfq temp = (*this) + x_ai;
      return temp;
    }


/*****************************************************************************/
    gfq& operator-=(
            const gfq& x   // gfq object x
    )                      // return: this = this - x
/*****************************************************************************/
    {
      (*this) = (*this) - x;
      return *this;
    }


/*****************************************************************************/
    gfq operator*(
            const gfq& x   // gfq object x
    ) const                // return: this * x
/*****************************************************************************/
    {
      gfq temp(Mult_table[value()][x.value()]);
      return temp;
    }


/*****************************************************************************/
    gfq& operator*=(
            const gfq& x   // gfq object x
    )                      // return: this = this * x
/*****************************************************************************/
    {
      (*this) = (*this) * x;
      return *this;
    }


/*****************************************************************************/
    gfq operator/(
            const gfq& x   // gfq object x
    ) const                // return: this * x^{-1]
/*****************************************************************************/
    {
      gfq v = x, temp;
      v.invert();
      temp = (*this) * v;
      return temp;
    }


/*****************************************************************************/
    gfq& operator/=(
            const gfq& x   // gfq object x
    )                      // return: this = this * x^{-1}
/*****************************************************************************/
    {
      (*this) = (*this) / x;
      return *this;
    }
};


ostream& operator<<(ostream& s, const gfq& x)
{
  s << x.value();
  return s;
}

/*****************************************************************************/
unsigned int rank_poly(
        polynomial<gfq>& x   // polynomial x over GF(q)
)                            // return: rank(x)
/*****************************************************************************/
{
  unsigned int tmp = 0;
  for (int i = 0; i <= x.degree(); i++)
    tmp += x[i].value() * power_n(field(),i);
  return tmp;
}


/*****************************************************************************/
polynomial<gfq> primitive_poly(
        unsigned int n           // natural number n
)                                // return: primitive polynomial with degree
                                 // n over GF(q)
/*****************************************************************************/
{
  unsigned int p = field();
  if ((p == 2) || (p == 4) || (p == 8) || (p == 16))
  {
    if (n == 2)
    {
      polynomial<gfq> tmp(2);
      tmp[0].init(1);tmp[1].init(1);tmp[2].init(1);
      return  tmp;
    }
    if (n == 3)
    {
      polynomial<gfq> tmp(3);
      tmp[0].init(1);tmp[1].init(1);tmp[3].init(1);
      return  tmp;
    }
    if (n == 4)
    {
      polynomial<gfq> tmp(4);
      tmp[0].init(1);tmp[3].init(1);tmp[4].init(1);
      return  tmp;
    }
    if (n == 5)
    {
      polynomial<gfq> tmp(5);
      tmp[0].init(1);tmp[2].init(1);tmp[5].init(1);
      return  tmp;
    }
    if (n == 6)
    {
      polynomial<gfq> tmp(6);
      tmp[0].init(1);tmp[5].init(1);tmp[6].init(1);
      return  tmp;
    }
    if (n == 7)
    {
      polynomial<gfq> tmp(7);
      tmp[0].init(1);tmp[6].init(1);tmp[7].init(1);
      return  tmp;
    }
    if (n == 8)
    {
      polynomial<gfq> tmp(8);
      tmp[0].init(1);tmp[2].init(1);tmp[3].init(1);
      tmp[4].init(1);tmp[8].init(1);
      return  tmp;
    }
    if (n == 9)
    {
      polynomial<gfq> tmp(9);
      tmp[0].init(1);tmp[4].init(1);tmp[9].init(1);
      return  tmp;
    }
    if (n == 10)
    {
      polynomial<gfq> tmp(10);
      tmp[0].init(1);tmp[3].init(1);tmp[10].init(1);
      return  tmp;
    }
    if (n == 11)
    {
      polynomial<gfq> tmp(11);
      tmp[0].init(1);tmp[2].init(1);tmp[11].init(1);
      return  tmp;
    }
    if (n == 12)
    {
      polynomial<gfq> tmp(12);
      tmp[0].init(1);tmp[1].init(1);tmp[4].init(1);
      tmp[6].init(1);tmp[12].init(1);
      return  tmp;
    }
    if (n == 13)
    {
      polynomial<gfq> tmp(13);
      tmp[0].init(1);tmp[1].init(1);tmp[3].init(1);
      tmp[4].init(1);tmp[13].init(1);
      return  tmp;
    }
    if (n == 14)
    {
      polynomial<gfq> tmp(14);
      tmp[0].init(1);tmp[1].init(1);tmp[3].init(1);
      tmp[5].init(1);tmp[14].init(1);
      return  tmp;
    }
    if (n > 15)
    {
      cout << "ERROR: primitive_poly(n): not implemented!\n";
      exit(1);
    }
  }
  if (p == 3)
  {
    if (n == 2)
    {
      polynomial<gfq> tmp(2);
      tmp[0].init(2);tmp[1].init(1);tmp[2].init(1);
      return  tmp;
    }
    if (n == 3)
    {
      polynomial<gfq> tmp(3);
      tmp[0].init(1);tmp[1].init(2);tmp[3].init(1);
      return  tmp;
    }
    if (n == 4)
    {
      polynomial<gfq> tmp(4);
      tmp[0].init(2);tmp[3].init(1);tmp[4].init(1);
      return  tmp;
    }
    if (n == 5)
    {
      polynomial<gfq> tmp(5);
      tmp[0].init(1);tmp[1].init(2);tmp[5].init(1);
      return  tmp;
    }
    if (n == 6)
    {
      polynomial<gfq> tmp(6);
      tmp[0].init(2);tmp[5].init(1);tmp[6].init(1);
      return  tmp;
    }
    if (n == 7)
    {
      polynomial<gfq> tmp(7);
      tmp[0].init(1);tmp[1].init(2);tmp[2].init(1);tmp[7].init(1);
      return  tmp;
    }
    if (n == 8)
    {
      polynomial<gfq> tmp(8);
      tmp[0].init(2);tmp[3].init(1);tmp[8].init(1);
      return  tmp;
    }
    if (n == 9)
    {
      polynomial<gfq> tmp(9);
      tmp[0].init(2);tmp[2].init(1);tmp[3].init(2);tmp[9].init(1);

      return  tmp;
    }
    if (n > 8)
    {
      cout << "ERROR: primitive_poly(n): not implemented!\n";
      exit(1);
    }
  }
  if (p == 5)
  {
    if ( n == 3)
    {
      polynomial<gfq> tmp(3);
      tmp[0].init(2);tmp[2].init(1);tmp[3].init(1);
      return  tmp;
    }
    if ( n == 4)
    {
      polynomial<gfq> tmp(4);
      tmp[0].init(2);tmp[1].init(3);tmp[3].init(1);tmp[4].init(1);
      return  tmp;
    }
  }
  if ( p!=2 && p!=3 && p!=5)
  {
    cout << "ERROR: primitive_poly(n): not implemented!\n";
    exit(1);
  }
}


/*****************************************************************************/
void set_field(
        unsigned int x    // prime power x
)                         // initializes the field, i.e. q := x, Add_table,
                          // Mult_table, Add_invert_table, Mult_invert_table
/*****************************************************************************/
{
  q = x;
  vector<int> pf;
  pf = prime_factorization(q);
  if (pf[0] != pf[pf.size()-1])
  {
    cout << "ERROR: set_field(q): q is not a prime power!\n";
    exit(1);
  }
  unsigned int n = pf.size(), p = pf[0];
  matrix<unsigned int> tmp_M(q,q), tmp_A(q,q);
  vector<unsigned int> tmp_Mi(q), tmp_Ai(q);
  if (n == 1)  // q is prime
  {
    tmp_Mi[0] = 0;
    for (unsigned int i = 0; i < q; i++)
      for (unsigned int j = i; j < q; j++)
      {
        tmp_A[i][j] = tmp_A[j][i] = (i+j) % q;
        tmp_M[i][j] = tmp_M[j][i] = (i*j) % q;
        if (tmp_M[i][j] == 1)
        {
          tmp_Mi[i] = j;
          tmp_Mi[j] = i;
        }
        if (tmp_A[i][j] == 0)
        {
          tmp_Ai[i] = j;
          tmp_Ai[j] = i;
        }
      }
  }
  else  // q real prime power
  {
    set_field(p);
    polynomial<gfq> ideal = primitive_poly(n);
    vector<vector<int> > el;
    vector<int> nv, lv;
    first_V_element(n,p,nv);
    el.push_back(nv);
    last_V_element(n,p,lv);
    while( nv != lv )
    {
      next_V_element(n,p,nv);
      el.push_back(nv);
    }
    tmp_Mi[0] = 0;
    for (unsigned int i = 0; i < x; i++)
      for (unsigned int j = i; j < x; j++)
      {
        polynomial<gfq> tmp_p1(n-1), tmp_p2(n-1);
        for (unsigned k = 0; k < n; k++)
        {
          tmp_p1[k].init(el[i][k]);
          tmp_p2[k].init(el[j][k]);
        }
        tmp_p1.simplify();
        tmp_p2.simplify();
        polynomial<gfq> tmp_p3;
        tmp_p3 = (tmp_p1 + tmp_p2) % ideal;
        tmp_A[i][j] = tmp_A[j][i] = rank_poly(tmp_p3);
        tmp_p3 = (tmp_p1 * tmp_p2) % ideal;
        tmp_M[i][j] = tmp_M[j][i] = rank_poly(tmp_p3);
        if (tmp_M[i][j] == 1)
        {
          tmp_Mi[i] = j;
          tmp_Mi[j] = i;
        }
        if (tmp_A[i][j] == 0)
        {
          tmp_Ai[i] = j;
          tmp_Ai[j] = i;
        }
      }
    q = x;
  }
  Add_table = tmp_A;
  Mult_table = tmp_M;
  Add_invert_table = tmp_Ai;
  Mult_invert_table = tmp_Mi;
}


/*****************************************************************************/
matrix<gfq> singer_matrix(
        unsigned int n      // natural number n (dimension)
)                           // return: generator of the singer subgroup
                            // of GL(n,q)
/*****************************************************************************/
{
  matrix<gfq> Gen(n,n);
  Gen.O();
  for (unsigned int i = 1; i < n; i++)
    Gen[i][i-1].init(1);
  polynomial<gfq> f = primitive_poly(n);
  for (unsigned int i = 0; i < n; i++)
    Gen[i][n-1] = -f[i];
  return Gen;
}


/*****************************************************************************/
matrix<gfq> frobenius_aut_matrix(
        unsigned int n             // natural number n (dimension)
)                                  // return: Frobenius matrix
/*****************************************************************************/
{
  polynomial<gfq> ideal = primitive_poly(n);
  matrix<gfq> Aut(n,n);
  for (unsigned int i = 0; i < n; i++)
  {
    polynomial<gfq> pol, im;
    pol.monom(i);
    im = pol;
    im.power_n(field());
    im = im % ideal;
    for (unsigned int j = 0; j <= im.degree(); j++)
      Aut[j][i] = im[j];
  }
  return Aut;
}


/*#############################################################################
##                                                                           ##
##                   TRANSVERSAL: vector of matrices                         ##
##                                                                           ##
#############################################################################*/


#define matrix_transversal vector<matrix<gfq> >
// a vector of matrices over GF(q)


/*****************************************************************************/
// out stream for matrix_transversal
ostream& operator<<(ostream& s, const matrix_transversal& T)
/*****************************************************************************/
{
  s << endl;
  for (unsigned int i = 0; i < T.size(); i++)
    cout << T[i] << endl;
  return s;
}


/****************************************************************************/
template<class T>
bool is_in_vector(
        const vector<T>& v,  // not sorted vector v
        const T& t           // Object t
)                            // return: true if t is in v else false
                             // used in the Dimino-Algorithm
/****************************************************************************/
{
  for (unsigned int i = 0; i < v.size(); i++)
  {
    if (t == v[i])
      return true;
  }
  return false;
}


/*#############################################################################
##                                                                           ##
##   GROUP: a matrix group is represented by a transversal chain             ##
##                                                                           ##
#############################################################################*/


/*****************************************************************************/
bool arg_is_well_known_group(
        char *arg             // string which contains the name of a group
)                             // return: true iff "arg" is a well known group
/*****************************************************************************/
{
  if ( strcmp(arg,"one") == 0 ||
       strcmp(arg,"diagonal") == 0 ||
       strcmp(arg,"center") == 0 ||
       strcmp(arg,"singer") == 0 ||
       strcmp(arg,"frobenius") == 0 ||
       strcmp(arg,"normalizersinger") == 0 ||
       strcmp(arg,"monomial") == 0 ||
       strcmp(arg,"monomialreduced") == 0 ||
       strcmp(arg,"generallinear") == 0 ||
       strcmp(arg,"borel") == 0 ||
       strcmp(arg,"specialupper") == 0 ||
       strcmp(arg,"symmetric") == 0 ||
       strcmp(arg,"cyclicperm") == 0)
    return true;
  else
    return false;
}


/*****************************************************************************/
bool arg_is_cyclic_group(
        char *arg             // string which contains the name of a group
)                             // return: true iff "arg" is a cyclic group
/*****************************************************************************/
{
  if ( strcmp(arg,"singer") == 0 ||
       strcmp(arg,"frobenius") == 0 ||
       strcmp(arg,"cyclic_perm") == 0)
    return true;
  else
    return false;
}


/*****************************************************************************/
bool arg_is_binop(
        char *arg    // "arg" is a binary operation
)                    // return: true iff "arg" is a binary operation of groups
/*****************************************************************************/
{
  if ( strcmp(arg,"x") == 0)
    return true;
  else
    return false;
}


class matrix_group: public vector<vector<matrix<gfq> > >
{
  public:

/*****************************************************************************/
  void all_elements(
    vector<matrix<gfq> >& v  // return: vector containing all group elements
  )
/*****************************************************************************/
  {
    v.clear();
    int d = size();
    vector<int> c(d,0);
    matrix<gfq> M;
    M = (*this)[d-1][c[d-1]];
    for (int i = d-1; i >= 0; i--)
      M *= (*this)[i][c[i]];
    v.push_back(M);
    while ( c[d-1] < (*this)[d-1].size() )
    {
      int z = 0;
      while ( z < d)
      {
        if (c[z] < (*this)[z].size() - 1)
	{
          c[z]++;
          M = (*this)[d-1][c[d-1]];
          for (int i = d-1; i >= 0; i--)
            M *= (*this)[i][c[i]];
          v.push_back(M);
	  z = 0;
	}
	else
	{
	  c[z] = 0;
	  z++;
	}
      }
      if (z == d) break;
    }
  }




/*****************************************************************************/
    void cyclic(
            const matrix<gfq>& M  // generator M
    )                             // return: this = <M>
/*****************************************************************************/
    {
      clear();
      unsigned long int i;
      matrix<gfq> tmp_M, Gen, Unit(M.rows(),M.rows());
      Unit.I();
      tmp_M = M;
      for (i = 1; Unit != tmp_M; i++)
        tmp_M *= M;
      vector<int> pf;
      pf = prime_factorization(i);
      vector<matrix<gfq> > tmp_T;
      assign(pf.size(),tmp_T);
      Gen = M;
      for (i = 0; i < pf.size(); i++)
      {
        if (M != Unit) operator[](i).push_back(Unit);
        tmp_M = Gen;
        operator[](i).push_back(tmp_M);
        for (unsigned long int k = 2; k < pf[i]; k++)
        {
          tmp_M *= Gen;
          operator[](i).push_back(tmp_M);
        }
        tmp_M *= Gen;
        Gen = tmp_M;
      }
    }


/*****************************************************************************/
    void generate(
            const vector<matrix<gfq> >& generator  // generator is a vector
    )                                              // with generators
                                                   // g_1,...,g_s
                                                   // return: this=<g_1,...g_s>
                                                   // DIMINIO algorithm
/*****************************************************************************/
    {
      clear();
      unsigned int reppos, prev_ord;
      vector<matrix<gfq> > element;
      matrix<gfq> Unit(generator[0].rows(),generator[0].rows()), tmp_M, g;
      Unit.I();
      element.push_back(Unit);
      g = generator[0];
      while ( g != Unit )
      {
        element.push_back(g);
        g *= generator[0];
      }
      for (unsigned int i = 1; i < generator.size(); i++)
      {
        if ( !is_in_vector(element,generator[i]) )
        {
          prev_ord = element.size();
          element.push_back(generator[i]);
          for (unsigned int j = 1; j < prev_ord; j++)
          {
            g = element[j] * generator[i];
            element.push_back(g);
          }
          reppos = prev_ord + 1;
          while ( reppos <= element.size() )
          {
            for (unsigned int j = 0; j <= i ; j++)
            {
              tmp_M = element[reppos-1] * generator[j];
              if ( !is_in_vector(element,tmp_M) )
              {
                element.push_back(tmp_M);
                for (unsigned int k = 1; k < prev_ord; k++)
                {
                  g = element[k] * tmp_M;
                  element.push_back(g);
                }
              }
            }
            reppos += prev_ord;
          }
        }
      }
      push_back(element);
    }


/*****************************************************************************/
    void one(
            unsigned int n  // natural number n (dimension)
    )                       // return: this = {1}, trivial subgroup
/*****************************************************************************/
    {
      clear();
      matrix<gfq> Unit(n,n);
      Unit.I();
      vector<matrix<gfq> > element;
      element.push_back(Unit);
      push_back(element);
    }



/*****************************************************************************/
    void diagonal(
            unsigned int n  // natural number n (dimension)
    )                       // return: this = { D | D is diagonal matrix }
/*****************************************************************************/
    {
      clear();
      matrix<gfq> Unit(n,n);
      Unit.I();
      for (unsigned int i = 0; i < n; i++)
      {
        vector<matrix<gfq> > element;
        for (unsigned int k = 0; k < field()-1; k++)
        {
          matrix<gfq> tmp_M = Unit;
          tmp_M[i][i].init(k+1);
          element.push_back(tmp_M);
        }
        push_back(element);
      }
    }


/*****************************************************************************/
    void center(
            unsigned int n  // natural number n (dimension)
    )                       // return: this = center of GL(n,q)
/*****************************************************************************/
    {
      clear();
      matrix<gfq> Unit(n,n);
      Unit.I();
      vector<matrix<gfq> > element;
      for (unsigned int k = 1; k < field(); k++)
      {
        matrix<gfq> tmp_M = Unit;
        for (unsigned int j = 0; j < n; j++)
          tmp_M[j][j].init(k);
        element.push_back(tmp_M);
      }
      push_back(element);
    }


/*****************************************************************************/
    void singer(
            unsigned int n  // natural number n (dimension)
    )                       // return: this = singer cycle of GL(n,q)
/*****************************************************************************/
    {
      clear();
      matrix<gfq> Gen;
      Gen = singer_matrix(n);
      cyclic(Gen);
    }


/*****************************************************************************/
    void frobenius(
            unsigned int n  // natural number n (dimension)
    )                       // return: this = frobenius automorphism of GL(n,q)
/*****************************************************************************/
    {
      clear();
      matrix<gfq> Gen;
      Gen = frobenius_aut_matrix(n);
      cyclic(Gen);
    }


/*****************************************************************************/
    void normalizer_singer(
            unsigned int n  // natural number n (dimension)
    )                       // return: this = normalizer singer cycle
/*****************************************************************************/
    {
      clear();
      matrix<gfq> F;
      F = frobenius_aut_matrix(n);
      vector<matrix<gfq> > v;
      singer(n);
      matrix<gfq> tmp_M(n,n);
      tmp_M.I();
      v.push_back(tmp_M);
      for (unsigned int i = 1; i < n; i++)
      {
        tmp_M *= F;
        v.push_back(tmp_M);
      }
      insert(begin(),v);
    }

/*****************************************************************************/
    void monomial_reduced(
            unsigned int n  // natural number n (dimension)
    )                       // return: this = M(n,q) = GF(q)* wr S_n
                            // the complete monomial group of GL(n,q)
/*****************************************************************************/
    {
      clear();
      matrix<gfq> Unit(n,n);
      Unit.I();
      for (unsigned int i = 0; i < n; i++)
      {
        vector<matrix<gfq> > element;
        for (unsigned int j = 0; j < n-i; j++)
        {
	  if (i == i+j) element.push_back(Unit);
	  else
	  {
            for (unsigned int k = 0; k < field()-1; k++)
            {
              matrix<gfq> tmp_M = Unit;
              tmp_M.permut_columns(i,i+j);
              tmp_M[i][i+j].init(k+1);
              element.push_back(tmp_M);
            }
	  }
        }
        push_back(element);
      }
    }

    
    

/*****************************************************************************/
    void monomial(
            unsigned int n  // natural number n (dimension)
    )                       // return: this = M(n,q) = GF(q)* wr S_n
                            // the complete monomial group of GL(n,q)
/*****************************************************************************/
    {
      clear();
      vector<matrix<gfq> > element;
      matrix<gfq> Unit(n,n), tmp_M;
      Unit.I();
      for (unsigned int i = 0; i < n; i++)
      {
        element.clear();
        for (unsigned int j = 0; j < n-i; j++)
        {
          tmp_M = Unit;
          tmp_M.permut_columns(i,i+j);
          element.push_back(tmp_M);
        }
        push_back(element);
      }
      if (field() > 2)
      {
        for (unsigned int i = 0; i < n; i++)
        {
          element.clear();
	  for (unsigned k = 1; k < field(); k++)
	  {
	    tmp_M = Unit;
	    tmp_M[i][i].init(k);
	    element.push_back(tmp_M);
     	  }
          push_back(element);
        }
      }
    }

/*****************************************************************************/
    void special_upper(
            unsigned int n  // natural number n (dimension)
    )                       // return: this = {M |M is upper triangular matrix}
/*****************************************************************************/
    {
      clear();
      matrix<gfq> Unit(n,n), tmp_M;
      Unit.I();
      for (unsigned int i = 0; i < n-1; i++)
      {
        vector<matrix<gfq> > transversal;
        vector<int> pgf, pgl;
        first_V_element(n-i-1,field(),pgf);
        last_V_element(n-i-1,field(),pgl);
        while ( pgf != pgl)
        {
          tmp_M = Unit;
          for (int r = 0; r < n-i-1; r++) tmp_M[i][i+1+r].init(pgf[r]);
          transversal.push_back(tmp_M);
          next_V_element(n-i-1,field(),pgf);
        }
        tmp_M = Unit;
        for (int r = 0; r < n-i-1; r++) tmp_M[i][i+1+r].init(pgl[r]);
        transversal.push_back(tmp_M);
        push_back(transversal);
      }
    }


/*****************************************************************************/
    void borel(
          unsigned int n  // natural number n (dimension)
    )                     // return: this = {M |M is upper triangular matrix}
/*****************************************************************************/
    {
      clear();
      matrix<gfq> Unit(n,n),tmp_M;
      Unit.I();
      for (unsigned int i = 0; i < n-1; i++)
      {
        vector<matrix<gfq> > transversal;
        for (unsigned int l = 1; l < field(); l++)
        {
          vector<int> pgf, pgl;
          first_PG_element(n-i-1,field(),pgf);
          last_PG_element(n-i-1,field(),pgl);
	  while ( pgf != pgl)
          {
            tmp_M = Unit;
            for (unsigned int r = 0; r < n-i-1; r++)
              tmp_M[i][i+1+r].init(pgf[r]);
            tmp_M[i][i].init(l);
            transversal.push_back(tmp_M);
            next_PG_element(n-i-1,field(),pgf);
          }
          tmp_M = Unit;
          for (unsigned int r = 0; r < n-i-1; r++)
            tmp_M[i][i+1+r].init(pgl[r]);
          tmp_M[i][i].init(l);
          transversal.push_back(tmp_M);
        }
        transversal.push_back(Unit);
        push_back(transversal);
      }
      for (unsigned int i = 0; i < n; i++)
      {
        vector<matrix<gfq> > transversal;
        for (unsigned int k = 0; k < field()-1; k++)
        {
          matrix<gfq> tmp_M = Unit;
          tmp_M[i][i].init(k+1);
          transversal.push_back(tmp_M);
        }
        push_back(transversal);
      }
    }


/*****************************************************************************/
    void general_linear(
            unsigned int n   // natural number n (dimension)
    )                        // return: this = GL(n,q)
/*****************************************************************************/
    {
      clear();
      matrix<gfq> Unit(n,n);
      Unit.I();
      for (unsigned int i = 0; i < n-1; i++)
      {
        vector<matrix<gfq> > element;
        vector<int> pgf, pgl;
        first_PG_element(n-i,field(),pgf);
        last_PG_element(n-i,field(),pgl);
        while (pgf != pgl)
        {
          matrix<gfq> tmp_M = Unit;
          for (unsigned int r = 0; r < n-i; r++) 
	    tmp_M[n-1-i][r].init(pgf[r]);
          for (unsigned int r = 0; r < n-i-1; r++)
          {
            tmp_M[r][n-1-i].init(1);
            if (pgf[r] != 0)
            {
              for (unsigned int m = r+1; m < n-i; m++)
              {
                if (pgf[m] != 0)
                {
                  tmp_M[r][n-i-1].init(0);
                  break;
                }
              }
            }
            else tmp_M[r][n-1-i].init(1);
          }
          element.push_back(tmp_M);
	  next_PG_element(n-i,field(),pgf);
        }
        push_back(element);
      }
      matrix_group B;
      B.borel(n);
      for (unsigned int i = 0; i < B.size(); i++) push_back(B[i]);
    }


/*****************************************************************************/
    void perm(
            unsigned int n  // natural number n (dimension)
    )                       // return: this = {1_q} wr S_n isomorphic to S_n
/*****************************************************************************/
    {
      clear();
      matrix<gfq> Unit(n,n);
      Unit.I();
      for (unsigned int i = 0; i < n; i++)
      {
        vector<matrix<gfq> > element;
        for (unsigned int j = 0; j < n-i; j++)
        {
          matrix<gfq> tmp_M = Unit;
          tmp_M.permut_columns(i,i+j);
          element.push_back(tmp_M);
        }
        push_back(element);
      }
    }


/*****************************************************************************/
    void cperm(
            unsigned int n  // natural number n (dimension)
    )                       // return: this = {1_q} wr C_n isomorphic to C_n
/*****************************************************************************/
    {
      clear();
      matrix<gfq> Gen(n,n);
      Gen.O();
      for (unsigned int i = 1; i < n; i++)
        Gen[i][i-1].init(1);
      Gen[0][n-1].init(1);
      cyclic(Gen);
    }
    

/*****************************************************************************/
    void direct_product(
            const matrix_group& A,  // matrix group A
            const matrix_group& B   // matrix group B
    )                               //                  --     --
                                    //                  | M | 0 | |  M in A
                                    // return: this = { | ----- | |   and   }
                                    //                  | 0 | N | |  N in B
                                    // this = A x B     --     --
/*****************************************************************************/
    {
      clear();
      unsigned int n1 = A.n(), n2 = B.n(), min, max;
      if (A.size() < B.size())
      {
        min = A.size();
        max = B.size();
      }
      else
      {
        min = B.size();
        max = A.size();
      }
      for (unsigned int i = 0; i < min; i++)
      {
        vector<matrix<gfq> > tmp_v;
        for (unsigned int l = 0; l < A[i].size(); l++)
          for (unsigned int k = 0; k < B[i].size(); k++)
          {
            matrix<gfq> tmp_M(n1+n2,n1+n2);
            tmp_M.O();
            for (unsigned int r = 0; r < n1 + n2; r++)
              for (unsigned int m = 0; m < n1 + n2; m++)
              {
                if (r < n1 && m < n1)
                  tmp_M[r][m] = (A[i][l])[r][m];
                if (r >= n1 && m >= n1)
                  tmp_M[r][m] = (B[i][k])[r-n1][m-n1];
              }
            tmp_v.push_back(tmp_M);
          }
        push_back(tmp_v);
      }
      for (unsigned int i = min; i < max; i++)
      {
        vector<matrix<gfq> > tmp_v;
        if (A.size() > B.size())
        {
          for (unsigned int l = 0; l < A[i].size(); l++)
          {
            matrix<gfq> tmp_M(n1+n2,n1+n2);
            tmp_M.I();
            for (unsigned int r = 0; r < n1 + n2; r++)
              for (unsigned int m = 0; m < n1 + n2; m++)
              {
                if (r < n1 && m < n1)
                  tmp_M[r][m] = (A[i][l])[r][m];
              }
            tmp_v.push_back(tmp_M);
          }
          push_back(tmp_v);
        }
        if (B.size() > A.size())
        {
          for (unsigned int l = 0; l < B[i].size(); l++)
          {
            matrix<gfq> tmp_M(n1+n2,n1+n2);
            tmp_M.I();
            for (unsigned int r = 0; r < n1 + n2; r++)
              for (unsigned int m = 0; m < n1 + n2; m++)
              {
                if (r >= n1 && m >= n1)
                  tmp_M[r][m] = (B[i][l])[r-n1][m-n1];
              }
            tmp_v.push_back(tmp_M);
          }
          push_back(tmp_v);
        }
      }
    }


/*****************************************************************************/
    void init_well_known_group(
            char *arg,          // name of the group arg
            unsigned int n      // natural number n (dimension)
    )                           // return: initializes this with the group
                                // of GL(n,q) saved in arg
/*****************************************************************************/
    {
      if ( strcmp(arg,"one") == 0 )
        one(n);
      if ( strcmp(arg,"diagonal") == 0 )
        diagonal(n);
      if ( strcmp(arg,"center") == 0 )
        center(n);
      if ( strcmp(arg,"singer") == 0 )
        singer(n);
      if ( strcmp(arg,"frobenius") == 0 )
        frobenius(n);
      if ( strcmp(arg,"normalizersinger") == 0 )
        normalizer_singer(n);
      if ( strcmp(arg,"monomial") == 0 )
        monomial(n);
      if ( strcmp(arg,"monomialreduced") == 0 )
        monomial_reduced(n);
      if ( strcmp(arg,"generallinear") == 0 )
        general_linear(n);
      if ( strcmp(arg,"borel") == 0 )
        borel(n);
      if ( strcmp(arg,"specialupper") == 0 )
        special_upper(n);
      if ( strcmp(arg,"symmetric") == 0 )
        perm(n);
      if ( strcmp(arg,"cyclicperm") == 0 )
        cperm(n);
    }


/*****************************************************************************/
    void init_binop(char *arg,      // name for the binary operation
            const matrix_group& A,  // matrix_group A
            const matrix_group& B   // matrix_group B
    )                               // return: this = arg(A,B)
/*****************************************************************************/
    {
      if ( strcmp(arg,"x") == 0 )
        direct_product(A,B);
    }


/*****************************************************************************/
    unsigned long int group_order() const // return: group order of this
/*****************************************************************************/
    {
      unsigned long int tmp = 1;
      for (unsigned long int i = 0; i < size(); i++)
        tmp *= operator[](i).size();
      return tmp;
    }


/*****************************************************************************/
    unsigned int n() const  // return: dimension n of elements in group
/*****************************************************************************/
    {
      return (*this)[0][0].rows();
    }


/*****************************************************************************/
    unsigned int memory() const // return: number of elements
                                // saved for this group
/*****************************************************************************/
    {
      unsigned tmp = 0;
      for (unsigned int i = 0; i < size(); i++)
        tmp += operator[](i).size();
      return tmp;
    }


};


/*#############################################################################
##                                                                           ##
##                              MATRIX LADDER                                ##
##                                                                           ##
##    computes the matrix ladder L_0,...,L_{2n-3]                            ##
##    containing the normalizers                                             ##
##    N_{GL(n,q)}(T_i) for i=0,1,3,...,n and                                 ##
##    N_{GL(n,q)}(T_i) cap N_{GL(n,q)}(T_{i-1}) for i=2,4,...,n              ##
##                                                                           ##
#############################################################################*/


class matrix_ladder: public vector<vector<matrix<gfq> > >
{
  public:


/*****************************************************************************/
    matrix_ladder() { }   // Standardconstructor
/*****************************************************************************/


/*****************************************************************************/
    matrix_ladder(            // constructor
            unsigned int n    // natural number n (dimension)
    )                         // return: "this" is a vector of transversals
                              // T_0,...,T_{2*n-4} where T_i is a transversal
                              // between L_i and L_{i+1}
/*****************************************************************************/
    {
      clear();
      unsigned int q = field();
      matrix<gfq> tmp_M(n,n),rI(n,n);
      rI.revers_I();  // rI = revers unit matrix
      vector<matrix<gfq> > tmp_T;
      assign(2*n-3,tmp_T);
      unsigned int i = 0, j;
      while( i < 2*n-4 )
      {
        j = n - (i + 1) / 2;  // transversal i
        vector<int> pg;
        operator[](i).clear();
        first_PG_element(j,q,pg);
        for (unsigned int k = 0; k < PG_order(j,q); k++) // next element
        {
          tmp_M.I();
          for (unsigned int l = 0; l < j; l++)
            tmp_M[j-1][l].init(pg[l]);
          for (unsigned int l = 0; l < j-1; l++)
          {
            tmp_M[l][j-1].init(1);
            if (pg[l] != 0)
            {
              for (unsigned int m = l+1; m < j; m++)
                if (pg[m] != 0)
                {
                  tmp_M[l][j-1].init(0);
                  break;
                }
            }
            else
              tmp_M[l][j-1].init(0);
          }
          operator[](i).push_back(tmp_M);
          if (k < PG_order(j,q)-1) next_PG_element(j,q,pg);
        }
        if (i > 0)  // transversal 2*n-3-i
        {
          for (unsigned int k = 0; k < operator[](i).size(); k++)
          {
            tmp_M = rI * operator[](i)[k];
            tmp_M *= rI;
            tmp_M.transpose();
            tmp_M.invert();
            operator[](2*n-3-i).push_back(tmp_M);
          }
        }
        if (i == 0)  // next i
          i++;
        else
          i += 2;
      }
    }

};


/*#############################################################################
##                                                                           ##
##                            LADDER GAME                                    ##
##                                                                           ##
##  computes a successively transversals D[0],...,D[2*n-4] of the double     ##
##  coset sets                                                               ##
##                                                                           ##
##       L_0 \ GL(n,q) / G,   . . .  ,  L_{2*n-3} \ GL(n,q) / G              ##
##                                                                           ##
##  where G is a subgroup of GL(n,q) and [L_0,...,L_{2*n-3}] is the matrix   ##
##  ladder.                                                                  ##
##                                                                           ##
##  There is a bijection between D[2*n-2*k-1] and a transversal of the       ##
##  orbits of G acting (from the left) on S(n,k,q), the set of k-subspaces   ##
##  of GF(q)^n:                                                              ##
##                                                                           ##
##                D[2*n-2*k-1]    <->    G \\ S(n,k,q)                       ##
##                                                                           ##
#############################################################################*/


/*****************************************************************************/
bool intest(           // intest_{L_{i+1}<L_i}
        unsigned int i,       // downstep number i
        const matrix<gfq>& g  // group element g of L_i
)                             // return: true if g is in L_{i+1} and
                              // false else
/*****************************************************************************/
{
  bool is_in = true;
  unsigned int j = g.rows() - (i + 1)/2;
  for (unsigned int k = 0; k <= j-2; k++)
  {
    if (g[j-1][k].value() != 0)
    {
      is_in = false;
      break;
    }
  }
  return is_in;
}


/*****************************************************************************/
unsigned int canonic_representative(
  unsigned int i,                  // downstep number i
  const matrix<gfq>& g             // group element g of L_i
)                                  // return: the index of the canonical
                                   // representative t in T_i such
                                   // that L_{i+1}g = L_{i+1}t
/*****************************************************************************/
{
  int k, last_pos;
  unsigned int j = g.rows()-(i+1)/2;
  vector<int> v(j,0);
  for (k = 0; k < j; k++)
    if (g[j-1][k].value() != 0) break;
  last_pos = k;
  gfq tmp1 = g[j-1][last_pos], tmp2;
  tmp1.invert();
  for (k = last_pos; k < j; k++)
  {
    tmp2 = g[j-1][k] * tmp1;
    v[k] = tmp2.value();
  }
  j = rank_PG_element(j,field(),v);
  return j;
}

/*****************************************************************************/
void stabilizer(

  unsigned int step,       // downstep number "step"

  const matrix_group& G,   // a group G

  const matrix<gfq>& g,    // group element C for the intest function:
                           // intest_{H<G}(h):=intest_{L_{i+1}<L_i}(ghg^{-1}),
                           // where  i := "step"

  matrix_group& H,         // return: a subgroup H of G given by the intest

  matrix_transversal& R    // return: transversal R of H\G
)
/*****************************************************************************/
{
  H.assign(G.size(),R);
  R.clear();

  matrix<gfq> g_inv = g,  id = g;
  g_inv.invert();
  id.I();
  R.push_back(id);
  for (unsigned long int i = G.size()-1; i >= 0 && i <= G.size()-1; i--)
  {
    unsigned long int T_size = R.size();
    for (unsigned long int j = 0; j < G[i].size(); j++)
    {
      for (unsigned long int k = 0; k < T_size; k++)
      {
        matrix<gfq> h = R[k] * G[i][j], m = g * h;
        m *= g_inv;
        if (intest(step,m)) H[i].push_back(h);
        else
        {
          matrix<gfq> x = h;
	  x.invert();
	  x *= g_inv;
          bool newcoset = true;
          for (unsigned long int l = 0; l < R.size(); l++)
          {
            m = g * R[l];
            m *= x;
            if (intest(step,m))
            {
              newcoset = false;
              break;
            }
          }
          if (newcoset) R.push_back(h);
        }
      }
    }
  }
  vector<vector<matrix<gfq> > >::iterator i = H.begin();
  ++i;
  while (i != H.end())
  {
    if (i->size() == 1) H.erase(i);
    else ++i;
  }
}


/*****************************************************************************/
bool is_down_step(
  unsigned int i     // step number i
)                    // return: true if i is a downstep else false
/*****************************************************************************/
{
  if (i % 2 == 1 || i == 0)
    return true;
  else
    return false;
}


/*****************************************************************************/
bool is_up_step(
  unsigned int i     // step number i
)                    // return: true if i is an upstep else false
/*****************************************************************************/
{
  if (i % 2 == 1 || i == 0)
    return false;
  else
    return true;
}


/*****************************************************************************/
unsigned int canonizer(

  unsigned int step,
     // step number "step"  (i:= "step")

  const matrix<gfq>& g,
     // group element g

  const matrix_ladder& N,
     // matrix ladder N

  const vector<matrix_transversal>& D,
     // double coset representatives D

  const vector<matrix<unsigned int> >& history_rep,
     // history I

  const vector<matrix<matrix<gfq> > >& history_fusel,
     // history II

  matrix<gfq>& u
     // return: group element u and
)    // return: index of a double coset element d of D[i] such that
     //         L_i*d*u = L_i*g
/*****************************************************************************/
{
  matrix<gfq> us, ds;
  unsigned int i = 0;
  u = D[0][i];
  for (unsigned int k = 0; k < step; k++)
  {
    ds = D[k][i];
    us = u;
    if (is_down_step(k))
    {
      matrix<gfq> tmp_g = ds * us;
      tmp_g.invert();
      tmp_g = g * tmp_g;
      unsigned int j = canonic_representative(k,tmp_g);
      u = history_fusel[k][i][j] * us;
      i = history_rep[k][i][j];
    }
    if (is_up_step(k))
    {
       u = history_fusel[k][i][0] * us;
       i = history_rep[k][i][0];
    }
  }
  return i;
}


/*****************************************************************************/
void down_step(

  unsigned int step,
      // step number "step"  (i:= "step")

  const matrix_ladder& N,
      // matrix ladder N

  const vector<matrix_group>& S_old,
      // vector S_old containing the stabilizers of the double cosets
      // represented by D[i]: S_old[j] is the stabilizer of the double
      // coset D[i][j]

  vector<matrix_group>& S_new,
      // return: vector S_new containing the stabilizers of the double cosets
      // represented by D[i+1]: S_new[j] is the stabilizer of the double
      // coset D[i+1][j]

  vector<matrix_transversal>& D,
      // double coset representatives D

  vector<matrix<unsigned int> >& history_rep,
      // history I

  vector<matrix<matrix<gfq> > >& history_fusel,
      // history II

  bool verbose
      // if (verbose == true) the print information on the screen
)
/*****************************************************************************/
{
  if (verbose)
    cout << "\n---------------------------------------------------\n"
         << "step " << step << ": downstep";
  matrix_transversal D_new;
  S_new.clear();
  unsigned long int ndc = 0;  //ndc = new double coset
  matrix<unsigned int> history_rep_new(D[step].size(),N[step].size());
  matrix<matrix<gfq> > history_fusel_new(D[step].size(),N[step].size());
  for (unsigned long int i = 0; i < D[step].size(); i++)
  {
    if (verbose) cout << "\nsplitting old double coset " << i
         << " into new double cosets\n";
    matrix<gfq> d_inv;
    d_inv = D[step][i];
    d_inv.invert();
    vector<bool> has_rep(N[step].size(),false);
    for (unsigned long int j = 0; j < N[step].size(); j++)
    {
      if (!has_rep[j])
      {
        D_new.push_back(N[step][j]*D[step][i]);
        matrix_group Stab;
        matrix_transversal R;
        stabilizer(step,S_old[i],D_new[ndc],Stab,R);
        S_new.push_back(Stab);
        if (verbose) cout << ndc << " ";
        for (unsigned long int k = 0; k < R.size(); k++)
        {
          matrix<gfq> tmp_g;
          tmp_g = D_new[ndc] * R[k];
          tmp_g *= d_inv;
          unsigned long int l = canonic_representative(step,tmp_g);
          has_rep[l] = true;
 	  history_rep_new[i][l] = ndc;
 	  history_fusel_new[i][l] = R[k];
        }
        ndc++;
      }
    }
  }
  D.push_back(D_new);
  D_new.clear();
  history_rep.push_back(history_rep_new);
  history_rep_new.clear();
  history_fusel.push_back(history_fusel_new);
  history_fusel_new.clear();
  if (verbose) cout << endl << ndc << " double cosets found\n";
}


/*****************************************************************************/
void up_step(

  unsigned int step,
      // step number "step"  (i:= "step")

  const matrix_ladder& N,
      // matrix ladder N

  const vector<matrix_group>& S_old,
      // vector S_old containing the stabilizers of the double cosets
      // represented by D[i]: S_old[j] is the stabilizer of the double
      // coset D[i][j]

  vector<matrix_group>& S_new,
      // return: vector S_new containing the stabilizers of the double cosets
      // represented by D[i+1]: S_new[j] is the stabilizer of the double
      // coset D[i+1][j]

  vector<matrix_transversal>& D,
      // double coset representatives D

  vector<matrix<unsigned int> >& history_rep,
      // history I

  vector<matrix<matrix<gfq> > >& history_fusel,
      // history II

  bool verbose
      // if (verbose == true) the print information on the screen

)
/*****************************************************************************/
{
  if (verbose)
    cout << "\n---------------------------------------------------\n"
         << "step " << step << ": upstep\n";
  matrix_transversal D_new;
  S_new.clear();
  unsigned int ndc = 0;  //ndc = new double coset
  vector<bool> history_check(D[step].size(),false);
  matrix<unsigned int> history_rep_new(D[step].size(),1);
  matrix<matrix<gfq> > history_fusel_new(D[step].size(),1);
  for (unsigned int i = 0; i < D[step].size(); i++)
  {
    vector<unsigned int> Rep;
    vector<matrix<gfq> > Fusel;
    unsigned int ii;
    for (unsigned int j = 0; j < N[step].size(); j++)
    {
      matrix<gfq> tmp;
      ii = canonizer(step,N[step][j]*D[step][i],N,D,
             history_rep,history_fusel,tmp);
      if (history_check[ii]) break;
      Rep.push_back(ii);
      Fusel.push_back(tmp);
    }
    if (!history_check[ii])
    {
      unsigned int i_0 = Rep[0];
      matrix<gfq> u_0 = Fusel[0];
      matrix_group Stab = S_old[i_0];
      vector<matrix<gfq> > R;
      matrix<gfq> tmp(D[step][0].rows(),D[step][0].rows());
      tmp.I();
      R.push_back(tmp);
      Stab.insert(Stab.begin(),R);
      if (verbose) cout << "fusing old double cosets\n";
      for (unsigned int k = 0; k < Rep.size(); k++)
      {
        tmp = Fusel[k];
        tmp.invert();
        tmp = u_0 * tmp;
        if (verbose) if (!history_check[Rep[k]]) cout << Rep[k] << " ";
   	history_check[Rep[k]] = true;
	history_rep_new[Rep[k]][0] = ndc;
	history_fusel_new[Rep[k]][0] = tmp;
	if (D[step][i_0] == D[step][Rep[k]] && Fusel[k] != u_0)
	{
	  tmp.invert();
	  Stab[0].push_back(tmp);
        }
      }
      D_new.push_back(D[step][i_0]);
      S_new.push_back(Stab);
      if (verbose) cout << "\nto new double coset " << ndc << endl;
      ndc++;
    }
  }
  D.push_back(D_new);
  D_new.clear();
  history_rep.push_back(history_rep_new);
  history_rep_new.clear();
  history_fusel.push_back(history_fusel_new);
  history_fusel_new.clear();
  if (verbose) cout << ndc << " double cosets found\n";
}


/*****************************************************************************/
void compute_only_orbits(

  unsigned int t,
     // dimension t

  const matrix_group& G,
     // subgroup G of GL(n,q)

  vector<matrix_transversal>& D,
     // return: double coset representatives D

  vector<matrix<unsigned int> >& history_rep,
     // return: history I

  bool verbose
     // if (verbose == true) the print information on the screen
)
/*****************************************************************************/
{
  if (verbose) cout << "ladder game\n";
  unsigned int n = G.n();
  history_rep.clear();
  D.clear();
  matrix_ladder N(n);
  matrix<gfq> id(n,n);
  id.I();
  matrix_transversal D_first;
  D_first.push_back(id);
  D.push_back(D_first);
  vector<matrix_group> S_old, S_new;
  S_old.push_back(G);
  vector<matrix<matrix<gfq> > > history_fusel;
  for (unsigned int i = 0; i < 2*n-2*t-1; i++)
  {
    if (is_down_step(i))
    {
      down_step(i,N,S_old,S_new,D,history_rep,history_fusel,verbose);
      if (i == 0) cout << D[i+1].size() << endl;
    }

    if (is_up_step(i))
    {
      up_step(i,N,S_old,S_new,D,history_rep,history_fusel,verbose);
      cout << D[i+1].size() << endl;
    }
    S_new.swap(S_old);
  }
  if (verbose) cout << "ladder game finished\n";
}


/*****************************************************************************/
void compute_orbits(

  unsigned int t,
     // dimension t

  const matrix_group& G,
     // subgroup G of GL(n,q)

  vector<matrix_transversal>& D,
     // return: double coset representatives D

  vector<matrix<unsigned int> >& history_rep,
     // return: history I

  vector<vector<unsigned int> >& stab_orders,
     // return: orders of the stabilizers of the double cosets:
     //         stab_orders[i][j] = order of the stabilizer of the double
     //                             coset D[i][j]

  bool verbose
     // if (verbose == true) the print information on the screen
)
/*****************************************************************************/
{
  if (verbose) cout << "ladder game\n";
  unsigned int n = G.n();
  stab_orders.clear();
  history_rep.clear();
  D.clear();
  matrix_ladder N(n);
  matrix<gfq> id(n,n);
  id.I();
  matrix_transversal D_first;
  D_first.push_back(id);
  D.push_back(D_first);
  vector<matrix_group> S_old, S_new;
  S_old.push_back(G);
  vector<unsigned int> ord_of_stab;
  ord_of_stab.push_back(G.group_order());
  stab_orders.push_back(ord_of_stab);
  vector<matrix<matrix<gfq> > > history_fusel;
  for (unsigned int i = 0; i < 2*n-2*t-1; i++)
  {
    if (is_down_step(i))
      down_step(i,N,S_old,S_new,D,history_rep,history_fusel,verbose);
    if (is_up_step(i))
      up_step(i,N,S_old,S_new,D,history_rep,history_fusel,verbose);
    ord_of_stab.assign(S_new.size(),0);
    for (unsigned j = 0; j < S_new.size(); j++)
      ord_of_stab[j] = S_new[j].group_order();
    stab_orders.push_back(ord_of_stab);
    S_new.swap(S_old);
  }
  if (verbose) cout << "ladder game finished\n";
}


/*#############################################################################
##                                                                           ##
##                               ORBITS                                      ##
##                                                                           ##
#############################################################################*/


/*****************************************************************************/
void make_echelon(
  matrix<gfq>& M  // rowwise generator matrix M
)                 // overwrites M with echelon form of the generator matrix
/*****************************************************************************/
{
  int n = M.columns(), k = M.rows(), row_pos = 0;
  for (int j = 0; j < n; j++)
  {
    bool col_is_zero = true;
    int p_i;
    gfq pivot;
    for (int i = row_pos; i < k; i++)
      if (M[i][j].value() != 0)
      {
        col_is_zero = false;
        p_i = i;
        pivot = M[i][j];
        break;
      }
    if (!col_is_zero)
    {
      if (row_pos != p_i) M.permut_rows(row_pos,p_i);
      for (int l = 0; l < n; l++)
        M[row_pos][l] /= pivot;
      for (int l = 0; l < k; l++)
      {
        if (l != row_pos)
          if (M[l][j].value() != 0)
	  {
	    pivot = M[l][j];
            for (int m = j; m < n; m++)
	      M[l][m] -= M[row_pos][m] * pivot;
	  }
      }
      row_pos++;
    }
  }
}


/*****************************************************************************/
matrix<gfq> kernel(
  matrix<gfq>& M       // matrix M
)                      // return: columnwise generator matrix
                       //         of the kernel of M
/*****************************************************************************/
{

  matrix<gfq> tmp_M = M;
  int n = M.columns(), k = M.rows(), row_pos = 0;
  vector<int> basis_cols;
  vector<bool> v(n,0);
  for (int j = 0; j < n; j++)
  {
    bool col_is_zero = true;
    int p_i;
    gfq pivot;
    for (int i = row_pos; i < k; i++)
      if (tmp_M[i][j].value() != 0)
      {
        col_is_zero = false;
	p_i = i;
	pivot = tmp_M[i][j];
	basis_cols.push_back(j);
	break;
      }
    if (!col_is_zero)
    {
      if (row_pos != p_i) tmp_M.permut_rows(row_pos,p_i);
      for (int l = 0; l < n; l++)
        tmp_M[row_pos][l] /= pivot;
      for (int l = 0; l < k; l++)
      {
        if (l != row_pos)
          if (tmp_M[l][j].value() != 0)
	  {
	    pivot = M[l][j];
            for (int m = j; m < n; m++)
	      tmp_M[l][m] = tmp_M[l][m] - tmp_M[row_pos][m]*pivot;
	  }
      }
      row_pos++;
    }
  }
  for (int i = 0; i < basis_cols.size(); i++)
    v[basis_cols[i]] = 1;  // v[k] = 1 iff  k is basis column
  int z = 0;
  matrix<gfq> N(n,n-basis_cols.size());
  N.O();
  for (int j = 0; j < n; j++)
  {
    if (v[j] == 0)
    {
      N[j][z].init(Add_invert_table[1]);
      for (int i = 0; i < basis_cols.size(); i++)
        N[basis_cols[i]][z] = tmp_M[i][j];
      z++;
    }
  }
  return N;
}


/*****************************************************************************/
matrix<gfq> dual_space(
  matrix<gfq>& M       // columnwise generator matrix M of a space S
)                      // return: columnwise generator matrix
                       //         of the dual space of S
/*****************************************************************************/
{
  matrix<gfq> N = M;
  N.transpose();
  return kernel(N);
}


/*****************************************************************************/
vector<matrix<gfq> > orbits(
  unsigned int k,                      // dimension k
  const vector<matrix_transversal>& D  // double cosets representatives
)                                      // return: transversal of the orbits
                                       // D[2*n-2*k-1] in vector space
                                       // representation, i.e. the elements
                                       // are k-subspaces
/*****************************************************************************/
{
  unsigned int n = D[0][0].rows();
  vector<matrix<gfq> > v;
  for (unsigned int i = 0; i < D[2*n-2*k-1].size(); i++)
  {
    matrix<gfq> D_inv_t, space(n,k);
    D_inv_t = D[2*n-2*k-1][i];
    D_inv_t.invert();
    for (unsigned int j = 0; j < k; j++)
      for (unsigned int l = 0; l < n; l++)
        space[l][j] = D_inv_t[l][j];
    space.transpose();
    make_echelon(space);
    space.transpose();
    v.push_back(space);
  }
  return v;
}


/*****************************************************************************/
void save_ASCII_orbits(

  unsigned int k,
     // dimension k

  char *group,
     // name for the group

  const vector<matrix_transversal>& D,
     // double coset representatives

  const vector<vector<unsigned int> >& stab_orders
     // orders of the stabilizers of the double cosets:
     // stab_orders[i][j] = order of the stabilizer of the double coset D[i][j]

)    // return: saves the corresponding orbits of "group" on S(n,k,q)
     //         in a ASCII file
/*****************************************************************************/
{
  unsigned int n = D[0][0].rows();
  vector<matrix<gfq> > v = orbits(k,D);
  char filename[256] = "Orbits_";
  char *tmp;
  tmp = new char[2];
  strcat(filename,group);
  strcat(filename,"_n");
  gcvt(n,2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_q");
  gcvt(field(),2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_k");
  gcvt(k,2,tmp);
  strcat(filename,tmp);
  delete [] tmp;
  strcat(filename,".txt");
  ofstream f(filename);
  f << "% Orbits\n";
  f << "% group = " << group << endl;
  f << "% n = " << n << endl;
  f << "% q = " << field() << endl;
  f << "% k = " << k << endl;
  f << "% number of orbits = " << v.size() << endl << endl;
  for (unsigned int i = 0; i < v.size(); i++)
  {
    f << i+1 << ".representative:" << v[i];
    f << "orbit length = "
      <<  stab_orders[0][0] / stab_orders[2*n-2*k-1][i] << endl << endl;
  }
  f.close();

}

/*****************************************************************************/
void save_ASCII_orbits_from_dual(

  unsigned int k,
     // dimension k

  char *group,
     // name for the group

  const vector<matrix_transversal>& D,
     // double coset representatives

  const vector<vector<unsigned int> >& stab_orders
     // orders of the stabilizers of the double cosets:
     // stab_orders[i][j] = order of the stabilizer of the double coset D[i][j]

)    // return: saves the corresponding orbits of "group" on S(n,k,q)
     //         in a ASCII file
/*****************************************************************************/
{
  unsigned int n = D[0][0].rows();
  vector<matrix<gfq> > v = orbits(n-k,D);
  char filename[256] = "Orbits_";
  char *tmp;
  tmp = new char[2];
  strcat(filename,group);
  strcat(filename,"_dual");
  strcat(filename,"_n");
  gcvt(n,2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_q");
  gcvt(field(),2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_k");
  gcvt(k,2,tmp);
  strcat(filename,tmp);
  delete [] tmp;
  strcat(filename,".txt");
  ofstream f(filename);
  f << "% Orbits\n";
  f << "% group = (" << group << ")^dual" << endl;
  f << "% n = " << n << endl;
  f << "% q = " << field() << endl;
  f << "% k = " << k << endl;
  f << "% number of orbits = " << v.size() << endl << endl;
  for (unsigned int i = 0; i < v.size(); i++)
  {
    f << i+1 << ".representative:" << dual_space(v[i]);
    f << "orbit length = "
      <<  stab_orders[0][0] / stab_orders[2*n-2*(n-k)-1][i] << endl << endl;
  }
  f.close();

}


/****************************************************************************/
void LaTeX_head(ofstream& f)
/****************************************************************************/
{
  f << "\\documentclass[a4paper]{article}\n";
  f << "\\begin{document}\n";
}


/****************************************************************************/
void LaTeX_foot(ofstream& f)
/****************************************************************************/
{
  f << "\\end{document}\n";
}


/*****************************************************************************/
void save_LaTeX_orbits_from_dual(

  unsigned int k,
     // dimension k

  char *group,
     // name for the group

  const vector<matrix_transversal>& D,
     // double coset representatives

  const vector<vector<unsigned int> >& stab_orders
     // orders of the stabilizers of the double cosets:
     // stab_orders[i][j] = order of the stabilizer of the double coset D[i][j]

)    // return: saves the corresponding orbits of "(group)^dual" on S(n,k,q)
     //         in a LaTeX file
/*****************************************************************************/
{
  int MAXROWS = 80;
  int MAXCOLS = 3;

  unsigned int n = D[0][0].rows();
  vector<matrix<gfq> > v = orbits(n-k,D);
  char filename[256] = "Orbits_";
  char *tmp;
  tmp = new char[2];
  strcat(filename,group);
  strcat(filename,"_dual");
  strcat(filename,"_n");
  gcvt(n,2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_q");
  gcvt(field(),2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_k");
  gcvt(k,2,tmp);
  strcat(filename,tmp);
  delete [] tmp;
  strcat(filename,".tex");
  ofstream f(filename);
  LaTeX_head(f);
  f << "\\section*{The Orbits $G\\backslash\\!\\!\\backslash S(" << n;
  f << "," << k << "," << q << ")$}\n";
  f << "$n=" << n << "$\\\\\n";
  f << "$q=" << q << "$\\\\\n";
  f << "$k=" << k << "$\\\\\n";
  f << "$G=(" << group << ")^\\perp$\\\\\n";
  f << "number of orbits = " << v.size() << "\\\\\\\\\n";

  f << "$\\tiny\\begin{array}{|c|c|c|}\n";
  f << "\\hline\n";
  f << "\\mbox{Nr.}&\\mbox{Rep.} &\\mbox{Length}\\\\\n";
  f << "\\hline\n";
  unsigned int m = 1;
  for (unsigned int i = 0; i < v.size(); i++)
  {
    if ( (i % (MAXROWS / n) == 0) && (i != 0) )
    {
      f << "\\end{array}$\n";
      if ( m % MAXCOLS == 0) f << "\\newpage\n";
      m++;
      f << "$\\tiny\\begin{array}{|c|c|c|}\n";
      f << "\\hline\n";
      f << "\\mbox{Nr.}&\\mbox{Rep.} &\\mbox{Length}\\\\\n";
      f << "\\hline\n";
    }
    f << i+1 << " & \n";
    f << "\\begin{array}{c}\n";      // i. representative
    for (int j = 0; j < n; j++)
    {
      for (int l = 0; l < k; l++)
      {
        f << dual_space(v[i])[j][l];
      }
      f << "\\\\\n";
    }
    f << "\\end{array} & ";

    f << stab_orders[0][0]/stab_orders[2*n-2*(n-k)-1][i] << "\\\\\n\\hline\n";
  }
  f << "\\end{array}$\n";
  LaTeX_foot(f);
  f.close();

}
/*#############################################################################
##                                                                           ##
##                   THE G-INCIDENCE MATRICES                                ##
##                                                                           ##
#############################################################################*/


/*****************************************************************************/
matrix<unsigned int> Mttp1(

  unsigned int n,
     // dimension n

  unsigned int t,
     // dimension t

  const vector<matrix<unsigned int> >& history_rep,
     // history I

  const vector<vector<unsigned int> >& stab_orders
     // orders of the stabilizers of the double cosets:
     // stab_orders[i][j] = order of the stabilizer of the double coset D[i][j]

)    // return: the G-incindence matrix M_{t,t+1}^G
/*****************************************************************************/
{  
  unsigned int i = 2*n - 2*t - 1;
  matrix<unsigned int> M_tmp(stab_orders[i].size(),stab_orders[i-2].size());
  M_tmp.O();
  for (unsigned int k = 0; k < stab_orders[i-2].size(); k++)
  {
    vector<bool> history_checked(stab_orders[i-1].size(),false);
    for (unsigned int j = 0; j < history_rep[i-2].columns(); j++)
    {
      unsigned int ii = history_rep[i-2][k][j];
      if (!history_checked[ii])
      {
        history_checked[ii] = true;
        unsigned int jj = history_rep[i-1][ii][0];
        M_tmp[jj][k] += stab_orders[i][jj] / stab_orders[i-1][ii];
      }
    }
  }
  return M_tmp;
}


/*****************************************************************************/
vector<matrix<unsigned int> > all_Mttp1(

  unsigned int n,
     // dimension n

  unsigned int t,
     // dimension t

  const vector<matrix<unsigned int> >& history_rep,
     // history I

  const vector<vector<unsigned int> >& stab_orders
     // orders of the stabilizers of the double cosets:
     // stab_orders[i][j] = order of the stabilizer of the double coset D[i][j]

)    // return: all M_{i,i+1}^G for t <= i <= n-1
/*****************************************************************************/
{
  vector<matrix<unsigned int> > kmmttp1;
  kmmttp1.clear();
  for (int unsigned tt = t; tt < n-1 ; tt++)
  {
    matrix<unsigned int> M_tmp = Mttp1(n,tt,history_rep,stab_orders);
    kmmttp1.push_back(M_tmp);
  }
  return kmmttp1;
}


/*****************************************************************************/
matrix<unsigned int> A_inf(

  unsigned int n,
     // dimension n

  unsigned int t,
     // dimension t

  unsigned int k,
     // dimension k

  const vector<matrix<unsigned int> >& kmmttp1
     // vector of all M_{i,i+1} for n-k <= i <= n-1

)    // return: A^inf_{t,k}
/*****************************************************************************/
{
  matrix<unsigned int> tmp_M = kmmttp1[0];
  for (unsigned int j = 1; j <= k - 1 - t; j++)
  {
    tmp_M *= kmmttp1[j];
    unsigned int gn = gauss_nkq(j + 1, 1, field());
    for (unsigned int i = 0; i < tmp_M.rows(); i++)
      for (unsigned int l = 0; l < tmp_M.columns(); l++)
        tmp_M[i][l] /= gn;
  }
  return tmp_M;
}


/*****************************************************************************/
matrix<unsigned int> A_sup(

  unsigned int n,
    // dimension n

  unsigned int k,
    // dimension k

  unsigned int t,
    // dimension t

  const vector<vector<unsigned int> >& stab_orders,
    // orders of the stabilizers of the double cosets:
    // stab_orders[i][j] = order of the stabilizer of the double coset D[i][j]

  const vector<matrix<unsigned int> >& kmmttp1
    // vector of all M_{i,i+1} for n-k <= i <= n-1

)   // return: A^sup_{k,t}
/*****************************************************************************/
{
  matrix<unsigned int> tmp_M;
  tmp_M = A_inf(n,t,k,kmmttp1);
  for (unsigned int i = 0; i < stab_orders[2*n-2*t-1].size(); i++)
    for (unsigned int j = 0; j < stab_orders[2*n-2*k-1].size(); j++)
    {
      tmp_M[i][j] *= stab_orders[2*n-2*k-1][j];
      tmp_M[i][j] /= stab_orders[2*n-2*t-1][i];
    }
  tmp_M.transpose();
  return tmp_M;
}


/*****************************************************************************/
void save_ASCII_A_inf_from_dual(

  unsigned int n,
    // dimension n

  unsigned int t,
    // dimension t

  unsigned int k,
    // dimension k

  char *group,
    // name of the group

  const vector<vector<unsigned int> >& stab_orders,
    // orders of the stabilizers of the double cosets:
    // stab_orders[i][j] = order of the stabilizer of the double coset D[i][j]

  const vector<matrix<unsigned int> >& kmmttp1
    // vector of all M_{i,i+1} for n-k <= i <= n-1

)   // return: compute and save A^inf_{t,k} from A^sup_{n-t,n-k}
/*****************************************************************************/
{
  matrix<unsigned int> M = A_sup(n,n-t,n-k,stab_orders,kmmttp1);
  char filename[256] = "A_inf_";
  char *tmp;
  tmp = new char[2];
  strcat(filename,group);
  strcat(filename,"_dual");
  strcat(filename,"_n");
  gcvt(n,2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_q");
  gcvt(field(),2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_t");
  gcvt(t,2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_k");
  gcvt(k,2,tmp);
  strcat(filename,tmp);
  delete [] tmp;
  strcat(filename,".txt");
  ofstream f(filename);
  f << "% A_inf\n";
  f << "% group = (" << group << ")^dual" << endl;
  f << "% n = " << n << endl;
  f << "% q = " << q << endl;
  f << "% t = " << t << endl;
  f << "% k = " << k << endl;
  f << "% number of rows = " << M.rows() << endl;
  f << "% number of columns = " << M.columns() << endl;
  f << M.rows() << " " << M.columns() << endl;
  f << M;
  f.close();
}


/*****************************************************************************/
void save_LaTeX_A_inf_from_dual(

  unsigned int n,
    // dimension n

  unsigned int t,
    // dimension t

  unsigned int k,
    // dimension k

  char *group,
    // name of the group

  const vector<vector<unsigned int> >& stab_orders,
    // orders of the stabilizers of the double cosets:
    // stab_orders[i][j] = order of the stabilizer of the double coset D[i][j]

  const vector<matrix<unsigned int> >& kmmttp1
    // vector of all M_{i,i+1} for n-k <= i <= n-1

)   // return: compute and save A^inf_{t,k} from A^sup_{n-t,n-k}
/*****************************************************************************/
{
  matrix<unsigned int> M = A_sup(n,n-t,n-k,stab_orders,kmmttp1);
  char filename[256] = "A_inf_";
  char *tmp;
  tmp = new char[2];
  strcat(filename,group);
  strcat(filename,"_dual");
  strcat(filename,"_n");
  gcvt(n,2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_q");
  gcvt(field(),2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_t");
  gcvt(t,2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_k");
  gcvt(k,2,tmp);
  strcat(filename,tmp);
  delete [] tmp;
  strcat(filename,".tex");
  ofstream f(filename);
  LaTeX_head(f);
  f << "\\section*{The Matrix $M_{" << t << "," << k << "}^G$}\n";
  f << "$n=" << n << "$\\\\\n";
  f << "$q=" << q << "$\\\\\n";
  f << "$t=" << t << "$\\\\\n";
  f << "$k=" << k << "$\\\\\n";
  f << "$G=(" << group << ")^\\perp$\\\\\n";
  f << "number of rows = " << M.rows() << "\\\\\n";
  f << "number of columns = " << M.columns() << "\\\\\n";
  f << "$$\\tiny\\begin{array}{c}\n";
  for (int i = 0; i < M.rows(); i++)
  {
    for (int j = 0; j < M.columns(); j++)
    {
      f << M[i][j];
    }
    f << "\\\\\n";
  }
  f << "\\end{array}$$\n";
  LaTeX_foot(f);
  f.close();
}


/*****************************************************************************/
void save_ASCII_blockingsetmatrix_all(

  unsigned int n,
    // dimension n

  unsigned int t,
    // dimension t

  char *group,
    // name of the group

  const vector<vector<unsigned int> >& stab_orders,
    // orders of the stabilizers of the double cosets:
    // stab_orders[i][j] = order of the stabilizer of the double coset D[i][j]

  const vector<matrix<unsigned int> >& kmmttp1,
    // vector of all M_{i,i+1} for n-k <= i <= n-1
    
  int Bmax
    // upper bound for the size of a blocking set

)   // return: compute and save A^inf_{t,k} from A^sup_{n-t,n-k}
/*****************************************************************************/
{
  matrix<unsigned int> M = A_sup(n,n-t,t,stab_orders,kmmttp1);
  char filename[256] = "BSM_";
  char *tmp;
  tmp = new char[2];
  strcat(filename,group);
  strcat(filename,"_n");
  gcvt(n,2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_q");
  gcvt(field(),2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_t");
  gcvt(t,2,tmp);
  strcat(filename,tmp);
  delete [] tmp;
  strcat(filename,".txt");
  matrix<int> bsm(M.rows(),M.columns()+M.rows()+2);
  bsm.O();
  for (int i = 0; i < M.rows(); i++)
  {
    bsm[i][M.columns()+i] = -1;
    for (int j = 0; j < M.columns(); j++) bsm[i][j] = M[i][j];
    bsm[i][bsm.columns()-1] = 1;
  }
  ofstream f(filename);
  f << "% Blocking set matrix\n";
  f << "% group = " << group << endl;
  f << "% n = " << n << endl;
  f << "% q = " << q << endl;
  f << "% t = " << t << endl;
  f << "% number of rows = " << bsm.rows()+1 << endl;
  f << "% number of columns = " << bsm.columns()-1 << endl;
  f << bsm.rows()+1 << " " << bsm.columns()-1 << " 1 ";
  f << bsm;
  for (int i = 0; i < stab_orders[2*t-1].size(); i++)
    f << stab_orders[0][0] / stab_orders[2*t-1][i] << " ";
  for (int i = 0; i < stab_orders[2*t-1].size(); i++) f << "0 ";
  f << "1 " << Bmax << endl;
  f << "BOUNDS " << bsm.columns()-1 << endl;
  for (int i = 0; i < bsm.rows(); i++) f << "1 ";
  for (int i = 0; i < bsm.rows(); i++) f << gauss_nkq(n-1,1,q)-2 << " ";
  f << Bmax << endl;
  f.close();
}

/*****************************************************************************/
void save_ASCII_blockingsetmatrix(

  unsigned int n,
    // dimension n

  unsigned int t,
    // dimension t

  char *group,
    // name of the group

  const vector<vector<unsigned int> >& stab_orders,
    // orders of the stabilizers of the double cosets:
    // stab_orders[i][j] = order of the stabilizer of the double coset D[i][j]

  const vector<matrix<unsigned int> >& kmmttp1,
    // vector of all M_{i,i+1} for n-k <= i <= n-1
    
  int Bmax
    // upper bound for the size of a blocking set

)   // return: compute and save A^inf_{t,k} from A^sup_{n-t,n-k}
/*****************************************************************************/
{
  matrix<unsigned int> M = A_sup(n,n-t,t,stab_orders,kmmttp1);
  char filename[256] = "BSM_";
  char *tmp;
  tmp = new char[2];
  strcat(filename,group);
  strcat(filename,"_n");
  gcvt(n,2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_q");
  gcvt(field(),2,tmp);
  strcat(filename,tmp);
  strcat(filename,"_t");
  gcvt(t,2,tmp);
  strcat(filename,tmp);
  delete [] tmp;
  strcat(filename,".txt");
  matrix<int> bsm(M.rows(),M.columns()+M.rows()+1);
  bsm.O();
  for (int i = 0; i < M.rows(); i++)
  {
    bsm[i][M.columns()+i] = -1;
    for (int j = 0; j < M.columns(); j++) bsm[i][j] = M[i][j];
  }
  ofstream f(filename);
  f << "% Blocking set matrix\n";
  f << "% group = " << group << endl;
  f << "% n = " << n << endl;
  f << "% q = " << q << endl;
  f << "% t = " << t << endl;
  f << "% Bmax = " << Bmax << endl;
  f << "% BOUND = " << gauss_nkq(n-1,1,q)-1 << endl;
  f << "% number of rows = " << M.rows()+1 << endl;
  f << "% number of columns = " << M.columns() << endl;
  f << M;
  for (int i = 0; i < stab_orders[2*n-2*t-1].size(); i++)
    f << stab_orders[0][0] / stab_orders[2*n-2*t-1][i] << " ";
  f << endl;
  f.close();
}


/*#############################################################################
##                                                                           ##
##                                  ORBITGRAPH                               ##
##                                                                           ##
#############################################################################*/


/*****************************************************************************/
int tcltk_koo_i(
  unsigned int lx,
  unsigned int ly,
  const vector<matrix_transversal>& D,
  unsigned int i,
  unsigned int j
)
/*****************************************************************************/
{
  return (int) (lx/D.size())/2 + i*lx/D.size();
}


/*****************************************************************************/
int tcltk_koo_j(
  unsigned int lx,
  unsigned int ly,
  const vector<matrix_transversal>& D,
  unsigned int i,
  unsigned int j
)
/*****************************************************************************/
{
  return (int)(ly/D[i].size())/2 + j*ly/D[i].size();
}


/*****************************************************************************/
void tcltk_orbitgraph(
  char *group,
  const vector<matrix_transversal>& D,
  const vector<matrix<unsigned int> >& history_rep,
  const vector<vector<unsigned int> >& stab_orders
) // generates the orbit graph as a Tcl/Tk file "orbitgraph.tcl"
/*****************************************************************************/
{
  unsigned int lx, ly;
  lx = 675;
  ly = 675;
  ofstream f("orbitgraph.tcl");
  f << "wm title . \"Orbit graph of group: " << group
    << "_q" << field() << "\"\n"
    << "button .button1 -text Dismiss -width 20 -command exit\n"
    << "canvas .c -relief raised -width 700 -height 700 -background white\n"
    << "pack .c -side top -fill x\n"
    << "set plotFont {Helvetica 10}\n";
  for (unsigned int i = 0; i < D.size(); i++)
    for (unsigned int j = 0; j < D[i].size(); j++)
    {
      f << ".c create oval "
        << tcltk_koo_i(lx,ly,D,i,j)-2 << " "
        << 700-(tcltk_koo_j(lx,ly,D,i,j)-2)-10 << " "
        << tcltk_koo_i(lx,ly,D,i,j)+2 << " "
        << 700-(tcltk_koo_j(lx,ly,D,i,j)+2)-10
        << " -width 1 -outline black -fill black\n";
      f << ".c create text "
        << tcltk_koo_i(lx,ly,D,i,j)-3 << " "
        << 700-(tcltk_koo_j(lx,ly,D,i,j)-5)-10 << " "
        << " -text \"(" << i << "," << j << ","
        << stab_orders[i][j] << ")\" -font $plotFont\n";
    }
  for (unsigned int i = 0; i < D.size()-1; i++)
    for (unsigned int j = 0; j < D[i].size(); j++)
    {
      vector<bool> history_checked(D[i+1].size(),false);
      for (unsigned int k = 0; k < history_rep[i][j].size(); k++)
      {
        if (!history_checked[history_rep[i][j][k]])
        {
          history_checked[history_rep[i][j][k]] = true;
          f << ".c create line "
            << tcltk_koo_i(lx,ly,D,i,j) << " "
            << 700-tcltk_koo_j(lx,ly,D,i,j)-10 << " "
            << tcltk_koo_i(lx,ly,D,i+1,history_rep[i][j][k]) << " "
            << 700-tcltk_koo_j(lx,ly,D,i+1,history_rep[i][j][k])-10
            << " -width 1\n";
        }
      }
    }
  f << "pack .button1 -fill y\n";
  f.close();
}

/*#############################################################################
##                                                                           ##
##                                  CODES                                    ##
##                                                                           ##
#############################################################################*/


/*****************************************************************************/
unsigned int griesmer_bound(
  int k,   // dimension of the code
  int q,   // order of the finite field
  int d    // minimum distance
)          // return: Griesmer bound for the length of the code
/*****************************************************************************/
{
  unsigned int gb = 0;
  for (int i = 0; i < k; i++)
  {
    if (d % power_n(q,i) == 0) gb += d / power_n(q,i);
    else gb += d / power_n(q,i) + 1;
  }
  return gb;
}




/*****************************************************************************/
unsigned int min_dist(
  const matrix<gfq>& M  // generator matrix of a code
)                       // return: minimum distance of the code
                        // simple algorithm: determines the weight
                        // of all code words
/*****************************************************************************/
{
  int n = M.columns(), k = M.rows();
  int d = M.columns();
  matrix <gfq> v(1,k);
  vector<int> w;
  first_V_element(k,field(),w);
  for (unsigned int i = 1; i < (unsigned int)pow((double)q,k); i++)
  {
    next_V_element(k,field(),w);
    for (int j = 0; j < k; j++)
      v[0][j].init(w[j]);
    matrix<gfq> tmp_M = v * M;
    unsigned int tmp_d = 0;
    for (int j = 0; j < n; j++)
      if (tmp_M[0][j].value() != 0)
        tmp_d++;
    if (tmp_d < d) d = tmp_d;
  }
  return d;
}


/*****************************************************************************/
void systematic_code(
  matrix<gfq>& M       // generator matrix of a code
)                      // return: the systematic generator matrix
/*****************************************************************************/
{
  int n = M.columns(), k = M.rows(), row_pos = 0;
  vector<int> basis_cols;
  for (int j = 0; j < n; j++)
  {
    bool col_is_zero = true;
    int p_i;
    gfq pivot;
    for (int i = row_pos; i < k; i++)
      if (M[i][j].value() != 0)
      {
        col_is_zero = false;
	p_i = i;
	pivot = M[i][j];
	basis_cols.push_back(j);
	break;
      }
    if (!col_is_zero)
    {
      if (row_pos != p_i) M.permut_rows(row_pos,p_i);
      for (int l = 0; l < n; l++)
        M[row_pos][l] /= pivot;
      for (int l = 0; l < k; l++)
      {
        if (l != row_pos)
          if (M[l][j].value() != 0)
	  {
	    pivot = M[l][j];
            for (int m = j; m < n; m++)
	      M[l][m] = M[l][m] - M[row_pos][m]*pivot;
	  }
      }
      row_pos++;
    }
  }
  for (int i = 0; i < k; i++)
    M.permut_columns(i,basis_cols[i]);
}


/*****************************************************************************/
float rate_of_binary_systematic_code(
  const matrix<gfq>& M  // systematic generator matrix M=(I|A)
)                       // return: how many entries of A are 1 (in percent)
/*****************************************************************************/
{
  int n = M.columns(), k = M.rows();
  float rate = 0;
  for (int i = 0; i < k; i++)
    for (int j = k; j < n; j++)
      if (M[i][j].value() != 0)
        rate = rate + 1;
  return 100 * rate / ((n-k)*k);
}



/*#############################################################################
##                                                                           ##
##                                       END                                 ##
##                                                                           ##
#############################################################################*/

