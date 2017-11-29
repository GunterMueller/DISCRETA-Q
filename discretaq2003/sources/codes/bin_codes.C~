#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

//typedef unsigned long int INT; 
//typedef vector<vector<INT> > matrix;

#define INT unsigned long int
#define matrix vector<vector<INT> > 

/*****************************************************************************/
void print_vector(const vector<INT>& v)
{
  cout << "[";
  for (INT i = 0; i < v.size() - 1; i++) 
    cout << v[i] << ",";
  cout << v[v.size() - 1] << "]";
}

/*****************************************************************************/
void scan_matrix(INT n, matrix& g)
{
  vector<INT> dummy(n);
  g.assign(n,dummy);  
  for (INT i = 0; i < n; i++)
    for (INT j = 0; j < n; j++)
      cin >> g[i][j];
}

/*****************************************************************************/
void print_matrix(const matrix& g)
{
  for (INT i = 0; i < g.size(); i++) 
  {
    print_vector(g[i]);
    cout << endl;
  }
}

/*****************************************************************************/
void transpose(const matrix& g, matrix& h)
{
  INT n = g.size();
  vector<INT> dummy(n);
  h.assign(n,dummy);
  for (INT i = 0; i < n; i++)
    for (INT j = 0; j < n; j++)
      h[i][j] = g[j][i];
}

/*****************************************************************************/
INT power(INT x, INT n)
{
  INT y = x, z = x, i = n - 1;
  if (n == 0) return 1;
  if ( z != 0 )
  {
    while ( i > 0 )
    {
      if ( (i % 2) == 0 )
      {
        y *= y;
        i >>= 1;
      }
       z *= y;
       i--;
    }
  }
  return z;
}

/*****************************************************************************/
INT theta(INT k)
{
  return (power(2,k) - 1);
}

/*****************************************************************************/
INT rank(vector<INT> v)
{
  INT x = 0;
  for (INT i = 0; i < v.size(); i++) 
    x += (v[i] * power(2,i)); 
  return (x - 1);
}

/*****************************************************************************/
void unrank(INT x, vector<INT>& v)
{
  INT i = 0, y = x + 1;
  while ( y > 0 )
  {
     v[i] = y % 2;
     i++;
     y >>= 1;
  }
  for (INT j = i; j < v.size(); j++) v[j] = 0;
}


/*****************************************************************************/
INT act(const matrix& g, INT x)
{
  INT k = g.size();
  vector<INT> v(k), w(k);
  unrank(x,v);
  for (INT i = 0; i < k; i++)
  {
     w[i] = 0;
     for (INT j = 0; j < k; j++) 
       w[i] += g[i][j] * v[j];
     w[i] = w[i] % 2;
  } 
  return rank(w);
}

/*****************************************************************************/
INT scalar_prod(INT k, INT x, INT y)
{
  vector<INT> v(k), w(k);
  unrank(x,v);
  unrank(y,w);
  INT s = 0;
  for (INT i = 0; i < k; i++)
    s += v[i] * w[i];
  return (s % 2);
}


/*****************************************************************************/
void orbits(const vector<matrix>& G, 
vector<INT>& v, vector<INT>& orb, vector<INT>& length)
{
  INT k = G[0].size(), theta_k = theta(k);
  orb.clear();
  length.clear();
  v.assign(theta_k,theta_k);

  for (INT i = 0; i < theta_k; i++)
  {
    if (v[i] == theta_k)
    {
      orb.push_back(i);
      length.push_back(1);
      v[i] = orb.size()-1;
      vector<INT> cand;
      cand.push_back(i);
      while ( cand.size() > 0 )
      {
        INT x = cand[cand.size()-1];
        cand.pop_back();
        for (INT j = 0; j < G.size(); j++)
        {
          INT y = act(G[j],x);
          if ( v[y] == theta_k )
          {
             v[y] = orb.size()-1;
             length[length.size()-1]++;
             cand.push_back(y);
          }
        }
      }
    }
  }
}

/*****************************************************************************/

void incidence_matrix(INT k, matrix& M, const vector<INT>& p_v,
const vector<INT>& p_orb, const vector<INT>& h_orb)
{
  INT l = p_orb.size(), theta_k = p_v.size();
  
  vector<INT> dummy(l);
  M.assign(l,dummy);
  for (INT i = 0; i < l; i++)
  {
    for (INT j = 0; j < theta_k; j++)
      if (scalar_prod(k,h_orb[i],j) == 0)
        M[i][p_v[j]]++;
  }
}




/*****************************************************************************/
int main(void)
{
  cout << "\n\n\nBINARY LINEAR CODE CONSTRUCTOR\n\n";

  INT k;
  cout << "Enter k = ";
  cin >> k;

  INT theta_k = theta(k);
  cout << "Number of points in PG(k-1,2) = " << theta_k << endl;

  INT nog; 
  cout << "\nEnter number of generators = ";
  cin >> nog;

  vector<matrix> G(nog), Gdual(nog);
  cout << "\nEnter generators row by row:\n";
  for (INT i = 0; i < nog; i++) 
  {
    cout << endl <<i << ". Generator:\n";
    scan_matrix(k,G[i]);
    transpose(G[i],Gdual[i]);
    cout << endl;
    print_matrix(G[i]);
  }

  cout << endl;
  
  vector<INT> p_v, p_orb, p_length, h_v, h_orb, h_length;

  orbits(G,p_v,p_orb,p_length);
  
  cout << "number of orbits = " << p_orb.size() << endl;

  orbits(Gdual,h_v,h_orb,h_length);

  matrix M;
  incidence_matrix(k,M,p_v,p_orb,h_orb);

  print_matrix(M);

  ofstream file("matrix.txt"); 

  file << "% n = " << k << endl;
  file << "% q = 2" << endl;
  file << "% number of rows = " << h_orb.size()+1 << endl;
  file << "% number of columns = " << p_orb.size() << endl << endl;
  for (INT i = 0; i < h_orb.size(); i++)
  {
    for (INT j = 0; j < p_orb.size(); j++)
      file << M[i][j] << " ";
    file << endl;
  }
  for (INT i = 0; i < p_orb.size(); i++)
    file << p_length[i] << " ";
  file << endl;
  file.close();
  


  




 
 
  return 0;
}
