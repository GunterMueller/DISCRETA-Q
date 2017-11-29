#include "discretaq.h"

using namespace std;





int main(int argc, char *argv[])
{
 
  if (argc < 3)
  {
    cout << "Too few parameters\n";
    exit(1);
  }

  int k = atoi(argv[1]);
  int q = atoi(argv[2]);
  int d = atoi(argv[3]);
  
  cout << "Griesmer bound = " << griesmer_bound(k,q,d) << endl;

  return 0;
}

