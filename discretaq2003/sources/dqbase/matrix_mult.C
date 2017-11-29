#include "discretaq.h"

int main(void)
{

  int n1,k1;

  cin >> n1;
  cin >> k1;

  matrix<int> A(n1,k1);

  int z;
  for (int i = 0; i < n1; i++)
    for (int j = 0; j < k1; j++)
    {
      cin >> z;
      A[i][j] = z;
    }

  cout << A;

  int n2,k2;

  cin >> n2;
  cin >> k2;

  matrix<int> B(n2,k2);

  for (int i = 0; i < n2; i++)
    for (int j = 0; j < k2; j++)
    {
      cin >> z;
      B[i][j] = z;
    }

  cout << B;
  
  matrix<int> C = A * B;

  cout << C;

  return 0;

}
