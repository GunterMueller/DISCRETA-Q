#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
  int i;
  char arg[100][100], order[256] = "";
  ifstream file(argv[1]);
  if (!file)
  {
    cout << "ERROR: File " << argv[1] << "does not exist\n";
    exit(1);
  }
  for (i = 0; i < 100; i++)
  {
    file >> arg[i];
    strcat(order,arg[i]);
    strcat(order," ");
    if (file.get() == EOF) break;
  }
  system(order);
  return 0;
}





