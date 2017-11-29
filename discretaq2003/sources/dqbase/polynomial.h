#ifndef __POLYNOMIAL_H
#define __POLYNOMIAL_H

// Michael Braun
// 2003.01.27
// polynomial.h

#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

/*#############################################################################
##                                                                           ##
##                               POLYNOMIAL                                  ##
##                                                                           ##
##  Polynomial are a_n*x^n + ... + a_1*x + a_0                               ##
##                                                                           ##
#############################################################################*/


template<class T>
class polynomial: public vector<T>
{

  public:


/****************************************************************************/
    polynomial<T>() { }  // Standardconstructor
/****************************************************************************/


/****************************************************************************/
    polynomial<T>(size_t n) : vector<T>(n+1) { }    // return: empty poly-
                                                    // nomial of degree n
/****************************************************************************/


/****************************************************************************/
    size_t degree() const { return size()-1; }  // return: degree of this
/****************************************************************************/


/****************************************************************************/
    polynomial<T>& O()  // return: this = 0*x^0 = 0
/****************************************************************************/
    {
      polynomial<T> tmp(0);
      tmp[0] = T(0);
      (*this) = tmp;
      return *this;
    }


/****************************************************************************/
    polynomial<T>& I()  // return: this = 1*x^0 = 1
/****************************************************************************/
    {
      polynomial<T> tmp(0);
      tmp[0] = T(1);
      (*this) = tmp;
      return *this;
    }


/****************************************************************************/
    polynomial<T>& monom(
            size_t n       // natural number n
    )                      // return: this = 1*x^n
/****************************************************************************/
    {
      polynomial<T> tmp(n);
      tmp[n] = T(1);
      (*this) = tmp;
      return *this;
    }


/****************************************************************************/
    void simplify() // deletes a_n if a_n = 0
/****************************************************************************/
    {
      size_t i = degree();
      while( (*this)[i] == T(0) && i > 0 )
      {
        i--;
        pop_back();
      }
    }


/****************************************************************************/
    polynomial<T> operator+(
            const polynomial<T>& p  // polynomial p
    ) const                         // return: this + p
/****************************************************************************/
    {
      int deg_max = (degree() < p.degree()) ? p.degree() : degree();
      polynomial<T> tmp(deg_max);
      for (size_t i = 0; i <= tmp.degree(); i++)
      {
        if ( i <= degree() )
          tmp[i] += (*this)[i];
        if ( i <= p.degree() )
          tmp[i] += p[i];
      }
      tmp.simplify();
      return tmp;
    }


/****************************************************************************/
    polynomial<T>& operator+=(
            const polynomial<T>& p  // polynomial p
     )                              // return: this = this + p
/****************************************************************************/
    {
      (*this) = (*this) + p;
      return *this;
    }


/****************************************************************************/
    polynomial<T> operator-(
            const polynomial<T>& p  // polynomial p
    ) const                         // return: this - p
/****************************************************************************/
    {
      int deg_max = (degree() < p.degree()) ? p.degree() : degree();
      polynomial<T> tmp(deg_max);
      for (size_t i = 0; i <= tmp.degree(); i++)
      {
        if ( i <= degree() )
          tmp[i] += (*this)[i];
        if ( i <= p.degree() )
          tmp[i] -= p[i];
      }
      tmp.simplify();
      return tmp;
    }


/****************************************************************************/
    polynomial<T>& operator-=(
            const polynomial<T>& p  // polynomial p
    )                               // return: this = this - p
/****************************************************************************/
    {
      (*this) = (*this) - p;
      return *this;
    }


/****************************************************************************/
    polynomial<T> operator*(
            const polynomial<T>& p  // polynomial p
    ) const                         // return: this * p
/****************************************************************************/
    {
      polynomial<T> tmp( degree() + p.degree() );
      for (size_t i = 0; i <= degree(); i++)
        for (size_t j = 0; j <= p.degree(); j++)
          tmp[i+j] += (*this)[i] * p[j];
      tmp.simplify();
      return tmp;
    }


/****************************************************************************/
    polynomial<T>& operator*=(
            const polynomial<T>& p  // polynomial p
    )                               // return: this = this * p
/****************************************************************************/
    {
      (*this) = (*this) * p;
      return *this;
    }


/****************************************************************************/
    polynomial<T> operator/(
            const polynomial<T>& p  // polynomial p
    ) const                         // There are q,r: this = p*q + r
                                    // return: q
/****************************************************************************/
    {
      polynomial<T> tmp, O;
      tmp.O();
      O.O();
      if ( degree() >= p.degree() )
      {
        polynomial<T> tmp_a = (*this), tmp_b = p;
        while( tmp_a.degree() >= tmp_b.degree() )
        {
          polynomial<T> tmp_p;
          tmp_p.monom(tmp_a.degree()-tmp_b.degree());
          tmp_p[tmp_a.degree()-tmp_b.degree()] =
            tmp_a[tmp_a.degree()] / tmp_b[tmp_b.degree()];
          tmp += tmp_p;
          tmp_p *= tmp_b;
          tmp_a -= tmp_p;
          if ( tmp_a == O )
            break;
        }
      }
      tmp.simplify();
      return tmp;
    }


/****************************************************************************/
    polynomial<T>& operator/=(
            const polynomial<T>& p  // polynomial p
    )                               // see "operator/": return: this = q
/****************************************************************************/
    {
      (*this) = (*this) / p;
      return *this;
    }


/****************************************************************************/
    polynomial<T> operator%(
            const polynomial<T>& p  // polynomial p
    ) const                         // see "operator/": return: r
/****************************************************************************/
    {
      polynomial<T> tmp1, tmp2;
      tmp1 = (*this) / p;
      tmp2 = (*this) - (p * tmp1);
      return tmp2;
    }


/****************************************************************************/
    polynomial<T>& power_n(
            unsigned long n    // natural number n
    )                          // return: this = this^n
/****************************************************************************/
    {
      polynomial<T> v = (*this), zero;
      zero.O();
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
};


/*****************************************************************************/
// out stream
template<class T>
ostream& operator<<(ostream& s, const polynomial<T>& v)
/*****************************************************************************/
{
  s << "[ ";
  for (size_t i = 0; i < v.degree(); i++)
    if (v[i] != T(0))
      s << v[i] << "*X^" << i << " , ";
  s << v[v.degree()] << "*X^" << v.degree() << " ]";
  return s;
}



#endif

