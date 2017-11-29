#include "discretaq.h"

using namespace std;

int main(int argc, char *argv[])
{
 
  if (argc < 5)
  {
    cout << "Too few parameters\n";
    exit(1);
  }

  int n = atoi(argv[1]);
  int k = atoi(argv[2]);
  int d = atoi(argv[3]);
  int q = atoi(argv[4]);
  cout << "[" << n << "," << k << "," << d << ";" << q << "]-Codes";
  cout << " <--> ";
  cout << "{" << (power_n(q,k)-1)/(q-1) - n << ",";
  cout << (power_n(q,k-1)-1)/(q-1) - n + d << ";" << k-1 << "," << q << "}-Minihypers\n";
  return 0;
}

