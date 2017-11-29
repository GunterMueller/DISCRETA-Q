
#include "discretaq.h"

using namespace std;

int main(int argc, char *argv[])

{

  ofstream codefile("optimal_codes.txt");
  codefile << "file = " << argv[4] << endl;
  int k = atoi(argv[1]);
  int q = atoi(argv[2]);
  int nbound = atoi(argv[3]);
  int d_max = power_n(q,k-1);
  char order1[256]=" discretaq_blockingsets -file ";
  strcat(order1,argv[4]);
  strcat(order1," -size ");

  for (int d = 1; d < d_max; d++)
  {
    int n = griesmer_bound(k,q,d);
    if (n>nbound) exit(1);
    if (gauss_nkq(k-1,1,q) - n + d == 0) exit(1);
    cout << "\n\n\n\n############################################\n";
    cout << "[" << n << "," << k << "," << d << ";" << q << "]-Codes <--> ";
    cout << "{" << gauss_nkq(k,1,q) - n << ",";
    cout << gauss_nkq(k-1,1,q) - n + d << ";" << k-1 << "," << q << "}-Minihypers\n";


    char order2[256]="";
    strcat(order2,order1);
    char *tmp;
    tmp = new char[10];
    gcvt(gauss_nkq(k,1,q) - n,10,tmp);
    strcat(order2,tmp);
    strcat(order2," -type ");
    gcvt(gauss_nkq(k-1,1,q) - n + d,10,tmp);
    strcat(order2,tmp);
    //strcat(order2," -break ");
    cout << order2 << endl;
    system(order2);
    cout << "[" << n << "," << k << "," << d << ";" << q << "]-Codes <--> ";
    cout << "{" << gauss_nkq(k,1,q) - n << ",";
    cout << gauss_nkq(k-1,1,q) - n + d << ";" << k-1 << "," << q << "}-Minihypers\n";
    ifstream solutionfile("BSM_minihypers.txt");
    if (solutionfile)
    {
      cout << "Loesungen gefunden\n";
      codefile << "[" << n << "," << k << "," << d << ";" << q << "]" << endl;
    }
    solutionfile.close();
    system("rm -f BSM_minihypers.txt");
    system("rm -f BSM_solutions1.txt");
    system("rm -f BSM_solutions2.txt");
  }
  codefile.close();
  return 0;

}

