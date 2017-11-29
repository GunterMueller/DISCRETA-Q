// Michael Braun
// 2003.01.15
// permuation.C


#include "permutation.h"


/*****************************************************************************/


permutation::permutation(const vector<unsigned int>& v)
{
  permutation p(v.size());
  for (int i = 0; i < v.size(); i++) p[i] = v[i];
  (*this) = p;
}


permutation::permutation(const vector<int>& v)
{
  permutation p(v.size());
  for (int i = 0; i < v.size(); i++) p[i] = (unsigned int)v[i];
  (*this) = p;
}


permutation& permutation::list(const vector<int>& v)
{
  permutation p(v);
  (*this) = p;
  return *this;
}


size_t permutation::degree() const 
{ 
  return size(); 
}


permutation& permutation::id(size_t n)
{
  permutation p(n);
  for (int i = 0; i < n; i++) p[i] = i;
  (*this) = p;
  return *this;
}


permutation& permutation::id()
{  
  for (int i = 0; i < degree(); i++) (*this)[i] = i;
  return *this;
}


permutation permutation::operator*(const permutation& p) const
{
  permutation t(degree());
  for (int i = 0; i < degree(); i++) t[i] = (*this)[p[i]];
  return t;
}
        

permutation& permutation::operator*=(const permutation& p)
{
  (*this) = (*this) * p;
  return *this;
}


permutation& permutation::invert()
{
  permutation t = (*this);
  for (int i = 0; i < degree(); i++) (*this)[t[i]] = i;
  return *this;
}


ostream& operator<<(ostream& s, const permutation& p)
{
  permutation t(p.degree());
  t.id();
  if ( p == t) s << "id";
  
  else
  {
    vector<bool> v(p.degree(),false);
    for (int i = 0; i < v.size(); i++)
    {
      if (!v[i])
      {
        if ( i!= p[i]) s << "(" << i;
        int j = i, k = i;
        while (p[k] != j)
        {
          v[k] = true;
          k = p[k];
          s << "," << k;
        }
        v[k] = true;
        if (i != p[i]) s << ")";
      }
    }
  }
  return s;
}


/*****************************************************************************/

