// Michael Braun
// 2003.01.15
// geometry.C


#include "geometry.h"


/*****************************************************************************/


void first_PG_element(int n,int q,vector<int>& x) 
{
  x.assign(n,0);
  x[n-1] = 1;
}


void next_PG_element(int n,int q,vector<int>& x)
{
  int i, pos = n-1, last_pos;
  for (i = 0; i < n; i++)
    if (x[i] == 1) break;
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


void last_PG_element(int n,int q,vector<int>& x)
{
  x.assign(n,q-1);
  x[0] = 1;
}


unsigned int PG_order(int n,int q)
{
  return ((unsigned int)pow((double)q,n)-1)/(q-1);
}



unsigned int rank_PG_element(int n,int q,const vector<int>& x)                              // return: a natural number
{
  unsigned int r;
  int i, pos = n-1, last_pos;
  for (i = 0; i < n; i++)
    if (x[i] == 1) break;
  last_pos = i;
  r = PG_order(n-last_pos-1,q);
  for (i = 0; i < n-last_pos-1; i++)
    r += x[n-1-i]*(unsigned int)pow((double)q,i);
  return r;
}


void first_V_element(int n,int q,vector<int>& x)
{
  x.assign(n,0);
}


void next_V_element(int n,int q,vector<int>& x)
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


void last_V_element(int n,int q,vector<int>& x)
{
  x.assign(n,q-1);
}


unsigned int rank_V(int n,int q,const vector<int>& x)                             // return:
{
  unsigned int tmp = 0;
  for (int i = 0; i < n; i++)
    tmp += x[i]*(unsigned int)pow((double)q,i);
  return tmp;
}

/*****************************************************************************/



