#include <fstream>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <vector>
#include "discretaq.h"

using namespace std;




int main(int argc, char* argv[])
{

  int size = 0, type = 1;
  char* filename;
  int n, q, t;

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
    cout << "discretaq_blockingsets -file [filename] -size [int] [-type [int]] [-minimal] [-break]\n";
    exit(1);
  }

  for (int m = 1; m < argc; m++)
  {
    if (strcmp(argv[m],"-minimal") == 0)
    {
      MINIMAL = true;
    }
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
    if (strcmp(argv[m],"-size") == 0)
    {
      if (m+1 == argc)
      {
        cout << "ERROR: what is the size of the blocking set?\n";
	exit(1);
      }
      size =  atoi(argv[m+1]);
      cout << "size = " << size << endl;
      if (size <= 1)
      {
        cout << "ERROR: what is the size of the blocking set?\n";
        exit(1);
      }
    }
    if (strcmp(argv[m],"-type") == 0)
    {
      if (m+1 == argc)
      {
        cout << "ERROR: what is the type of the blocking set?\n";
	exit(1);
      }
      type =  atoi(argv[m+1]);
      cout << "type = " << type << endl;
      if (type < 1)
      {
        cout << "ERROR: what is the type of the blocking set?\n";
        exit(1);
      }
    }
  }

  if (size <= 1)
  {
    cout << "ERROR: what is the size of the blocking set?\n";
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

  int bound, Bmax, no_rows, no_cols;

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

    if (strstr(zeile,"% Bmax = ") != 0)
    {
     Bmax = atoi((strstr(zeile,"% Bmax = "))+9);
     cout << "Bmax = " << Bmax << endl;
    }

    if (strstr(zeile,"% n = ") != 0)
    {
     n = atoi((strstr(zeile,"% n = "))+6);
     cout << "n = " << n << endl;
    }

    if (strstr(zeile,"% t = ") != 0)
    {
     t = atoi((strstr(zeile,"% t = "))+6);
     cout << "t = " << t << endl;
    }

    if (strstr(zeile,"% q = ") != 0)
    {
     q = atoi((strstr(zeile,"% q = "))+6);
     cout << "q = " << q << endl;
    }

    if (strstr(zeile,"% BOUND = ") != 0)
    {
     bound = atoi((strstr(zeile,"% BOUND = "))+10);
     cout << "BOUND = " << bound << endl;
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
    if (i < no_rows-1) newf1 << type << "\n";
    else newf1 << size << endl;
  }
  newf1 << "BOUNDS " << no_cols + no_cols << endl;
  for (int j = 0; j < no_cols; j++) newf1 << "1 ";
  for (int j = 0; j < no_cols; j++) newf1 << bound - type +1  << " ";
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
  bool is_minihyper = false;
  while(!solf1.eof())
  {
    index++;
    solf1 >> z;
    if ((index <= no_cols) && (!solf1.eof())) solf2 << z << " ";
    if (index > no_cols) if (z == 0) is_minihyper = true;
    if (index == 2 * no_cols)
    {
      if (is_minihyper) 
      {  
        no_of_minihypers++;
        solf2 << "+" << endl;
	if (ABBRUCH) 
	{
	  cout << "minihyper found\n";
	  solf2.close();
	  exit(1);
	}
      }
      else solf2 << "-" << endl;
      no_sols++;
      index = 0;
      is_minihyper = false;
    }
  }

  solf1.close();
  solf2.close();

  system("rm -f solutions");
  
  
  
  if (MINIMAL)
  {

/*----------------------------------------------------------------
 Durchlaufe nun alle Loesungen aus der Datei "BSM_solutions1.txt"
 und suche diejenigen heraus die Bahnen-minimial sind.
----------------------------------------------------------------*/

    ifstream solf3("BSM_solutions1.txt");
    ofstream solf4("BSM_solutions2.txt");
    for (unsigned long int i = 0; i < no_sols; i++)
    {
      vector<int> v(no_cols);
      int gewicht = 0;
      for (unsigned long int j = 0; j < no_cols; j++)
      {
        solf3 >> v[j];
        cout << v[j] << " ";
        if (v[j] == 1) gewicht++;
      }
      char ch;
      solf3 >> ch;
      cout << " * " << gewicht << endl;
      ofstream newf2("BSM_matrix2.txt");
      newf2 << no_rows << " " << gewicht + no_rows  << " 1\n";
      for (int l = 0; l < no_rows-1; l++)
      {
        for (int k = 0; k < no_cols; k++) if (v[k] == 1) newf2 << M[l][k] << " ";
        for (int k = 0; k < no_rows-1; k++)
        {
          if (k == l) newf2 << "-1 ";
	  else newf2 << "0 ";
        }
        newf2 << "0 1\n";
      }
      for (int k = 0; k < no_cols; k++)
        if (v[k] == 1) newf2 << M[no_rows-1][k] << " ";
      for (int k = 0; k < no_rows-1; k++) newf2 << "0 ";
      newf2 << "1 " << size-1 << endl;
      newf2 << "BOUNDS " << gewicht + no_rows << endl;
      for (int l = 0; l < gewicht; l++)
        newf2 << "1 ";
      for (int l = 0; l < no_rows - 1; l++)
        newf2 << bound << " ";
      newf2 << size-1 << endl;
      newf2.close();
      system("discretaqsolve -c0factor 1000 -bkz 80 18 -file BSM_matrix2.txt -silent");
      system("cat solutions");
      ifstream solf5("solutions");
      solf5.getline(zeile,10000);
      solf5 >> z;
      cout << z << endl;
      if (z == 0)
      {
        no_orbit_minimal_sols++;
        for (unsigned long int j = 0; j < no_cols; j++)
          solf4 << v[j] << " ";
        solf4 << endl;
      }
      solf5.close();
    }
    solf3.close();
    solf4.close();
  }

/*----------------------------------------------------------------
 Die temporaeren Dateien werden wieder geloescht.
----------------------------------------------------------------*/

  system("rm -f solutions");
 // system("rm -f BSM_matrix1.txt");
  system("rm -f BSM_matrix2.txt");

/*----------------------------------------------------------------
 Die Ergebnisse werden auf dem Bildschirm ausgegeben.
----------------------------------------------------------------*/

  cout << "\n\n\n";
  cout << "****************************************************************\n\n";
  cout << "The number of ";
  cout << type;
  cout << "-blocking sets in PG(";
  cout << n-1;
  cout << ",";
  cout << q;
  cout << ") with ";
  cout << groupname << endl;
  cout << "as a group of automorphisms and the size ";
  cout << size;
  cout << " is ";
  cout << no_sols;
  cout << ". \n";
  cout << "The number of minihypers is " << no_of_minihypers << ".\n";
  if (no_of_minihypers > 0)
  {
    ofstream solf5("BSM_minihypers.txt");
    solf5 << no_of_minihypers << endl;
    solf5.close();
  }
  if (MINIMAL)
  {
    cout << "The number of orbit minimal blocking sets is ";
    cout << no_orbit_minimal_sols << "." << endl << endl;
  }
  return 0;
}
