#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "discretaq.h"

using namespace std;




int main(int argc, char* argv[])
{


  char* filename;
  int bound = 1, n = 0, d = 1, k, q, t;

  bool MINIMAL = false;

  bool ABBRUCH = false;

  unsigned long int no_orbit_minimal_sols = 0;

  char groupname[256] = "";

/*--------------------------
   Befehlszeile einlesen
--------------------------*/

  if (argc <= 3)
  {
    cout << "Syntax\n";
    cout << "discretaq_codes -file [filename] -n [int] -d [int]\n";
    exit(1);
  }

  for (int m = 1; m < argc; m++)
  {
    if (strcmp(argv[m],"-break") == 0)
    {
      ABBRUCH = true;
    }
    if (strcmp(argv[m],"-file") == 0)
    {
      if (m+1 == argc)
      {
        cout << "ERROR: what is the file?\n";
	exit(1);
      }
      filename = argv[m+1];
      cout << filename << endl;
    }
    if (strcmp(argv[m],"-n") == 0)
    {
      if (m+1 == argc)
      {
        cout << "ERROR: what is the length of the code?\n";
	exit(1);
      }
      n =  atoi(argv[m+1]);
      cout << "n = " << n << endl;
      if (n <= 1)
      {
        cout << "ERROR: what is the length of the code?\n";
        exit(1);
      }
    }
    if (strcmp(argv[m],"-d") == 0)
    {
      if (m+1 == argc)
      {
        cout << "ERROR: what is the minimum distance of the code?\n";
	exit(1);
      }
      d =  atoi(argv[m+1]);
      cout << "d = " << d << endl;
      if (d < 1)
      {
        cout << "ERROR: what is the minimum distance of the code?\n";
        exit(1);
      }
    }
    if (strcmp(argv[m],"-bound") == 0)
    {
      if (m+1 == argc)
      {
        cout << "ERROR: what is the bound?\n";
	exit(1);
      }
      bound =  atoi(argv[m+1]);
      cout << "bound = " << bound << endl;
      if (bound < 1)
      {
        cout << "ERROR: what is the bound?\n";
        exit(1);
      }
    }
  }

  if (n <= 1)
  {
    cout << "ERROR: what is the length of the code?\n";
    exit(1);
  }

/*-------------------------------------------------------------
 Ab hier wird das Diophantische Gleichungssystem
 BSM1.txt angegeben. Die Loesungen von BSM1.txt
 entsprechen den Blocking Sets mit Blockgroesse "size".
 ------------------------------------------------------------*/

  ifstream oldf(filename);
  if (!oldf)
  {
    cout << "ERROR: file " << filename << " does not exist\n";
    exit(1);
  }

  int no_rows, no_cols;

  // ab hier wird der Vorspann der datei eingelesen:

  char zeile[10000];

  bool vorspann = true;
  while (vorspann)
  {
    oldf.getline(zeile,10000);
    if (zeile[0] != '%') vorspann = false;

    if (strstr(zeile,"% group = ") != 0)
    {
     strcat(groupname,strstr(zeile,"% group = ")+10);
     cout << "group = " << groupname << endl;
    }
    if (strstr(zeile,"% n = ") != 0)
    {
     k = atoi((strstr(zeile,"% n = "))+6);
     cout << "k = " << k << endl;
    }
    if (strstr(zeile,"% q = ") != 0)
    {
     q = atoi((strstr(zeile,"% q = "))+6);
     cout << "q = " << q << endl;
    }
    if (strstr(zeile,"% number of rows = ") != 0)
    {
     no_rows = atoi((strstr(zeile,"% number of rows = "))+19);
     cout << "number of rows = " << no_rows << endl;
    }

    if (strstr(zeile,"% number of columns = ") != 0)
    {
     no_cols = atoi((strstr(zeile,"% number of columns = "))+22);
     cout << "number of columns = " << no_cols << endl;
    }
  }

  // hier wird jetzt die Matrix aufgebaut:

  ofstream newf1("BSM_matrix1.txt");

  int z;

  matrix<int> M(no_rows,no_cols);

  newf1 << no_rows << " " << no_cols + no_cols << " 1\n";

  for (int i = 0; i < no_rows; i++)
  {
    for (int j = 0; j < no_cols; j++)
    {
      oldf >> z;
      M[i][j] = z;
      newf1 << z << " ";
    }
    for (int j = 0; j < no_cols; j++)
    {
      if (i == j ) newf1 << "-1 ";
      else newf1 << "0 ";
    }
    if (i < no_rows-1) newf1 << 0 << "\n";
    else newf1 << n << endl;
  }
  newf1 << "BOUNDS " << no_cols + no_cols << endl;
  for (int j = 0; j < no_cols; j++) newf1 << bound << " ";
  for (int j = 0; j < no_cols; j++) newf1 << n-d  << " ";
  newf1 << endl;

  oldf.close();
  newf1.close();

/*----------------------------------------------------------
 Hier wird das Gleichungssystem BSM_matrix1.txt geloest und
 die Loesungen werden in der Datei "solutions" gespeichert.
----------------------------------------------------------*/

  system("discretaqsolve -c0factor 1000 -bkz 80 18 -file BSM_matrix1.txt");

/*------------------------------------------------------------
 Die Loesungen aus "solutions" werden nun bei der Haelfte
 abgeschnitten und in der neuen Datei "BSM_solutions1.txt"
 abgespeichert.
------------------------------------------------------------*/

  ifstream solf1("solutions");
  ofstream solf2("BSM_solutions1.txt");

  int index = 0;
  unsigned long int no_sols = 0;
  unsigned long int no_of_minihypers = 0;
  vector<unsigned long int> dummy(n-d+1);
  vector<unsigned long int> invariante;
  bool is_code = false;
  while(!solf1.eof())
  {
    index++;
    solf1 >> z;
    if ((index <= no_cols) && (!solf1.eof())) solf2 << z << " ";
    if (index == no_cols) invariante = dummy;
    if (index > no_cols)
    {
      if (z == n-d) is_code = true;
      invariante[z]++;
    }
    if (index == 2 * no_cols)
    {
      if (is_code)
      {
        no_of_minihypers++;
        solf2 << "+" << endl;
	if (ABBRUCH)
	{
	  cout << "code found\n";
	  solf2.close();
	  exit(1);
	}
	unsigned long int invar = 0;
	for (unsigned long int i = 1; i <= n-d; i++)
	  invar += (invariante[i] * i * power_n(n-d,i));
	cout << invar << endl;
      }
      else solf2 << "-" << endl;
      no_sols++;
      index = 0;
      is_code = false;
    }
  }

  solf1.close();
  solf2.close();

  system("rm -f solutions");
  

/*----------------------------------------------------------------
 Die temporaeren Dateien werden wieder geloescht.
----------------------------------------------------------------*/

  system("rm -f solutions");
  system("rm -f BSM_matrix2.txt");

/*----------------------------------------------------------------
 Die Ergebnisse werden auf dem Bildschirm ausgegeben.
----------------------------------------------------------------*/

  cout << "\n\n\n";
  cout << "****************************************************************\n\n";
  cout << no_sols;
  cout << "****************************************************************\n\n";
  return 0;
}


