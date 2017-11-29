#include "discretaq.h"

int main(void)
{

  int n,k,q;



  cin >> n;
  cin >> k;
  cin >> q;


  cout << "n = " << n << endl;
  cout << "k = " << k << endl;
  cout << "q = " << q << endl;

  set_field(q);
  
  print_tables();

  matrix<gfq> G(k,n);

  int z;
  for (int i = 0; i < k; i++)
    for (int j = 0; j < n; j++)
    {
      cin >> z;
      G[i][j].init(z);
    }

  cout << G;

  systematic_code(G);

  cout << G;

  cout << " the minimum distance of the code is " << min_dist(G) << endl;

  return 0;

}
