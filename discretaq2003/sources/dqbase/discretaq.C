// Michael Braun
// 2003.01.16

//compute_incidence_matrix.C
//a program to compute the incidence matrix M^G_{t,k}

#include "discretaq.h"

using namespace std;

int main(int argc, char *argv[])
{

/*--------------------------
           variables
--------------------------*/

  int q, n;                                  //the prescribed group is
                                             //a subgroup of GL(n,q)

  int t = 1, k;                              //parameters for matrix

  char group[256] = "";                      //string for groupname

  char filename[256] = "";                   //string for filename for matrix

  matrix_group G;                            //the precribed group G

  vector<matrix_transversal> D;              //double coset representatives

  vector<matrix<unsigned int> > history_rep; //history of laddergame

  vector<vector<unsigned int> > stab_orders; //orders of stabilizers
                                             //of double coset reps.

  vector<matrix<unsigned int> > kmmttp1;     //all matrices M^G_{i,i+1}
                                             //for i=t,...,n-2

  matrix<unsigned int> Asup;                 // A_inf-matrix

  int Bmax;                                  // upper bound for the size
                                             // of blocking sets

  bool verbose = false;                      // internal outputs


/*--------------------------
           help
--------------------------*/

  if ( (argc == 1) || (strcmp(argv[1],"-help") == 0) )
  {
    cout
    << "/*#############################################################################\n"
    << "##                                                                           ##\n"
    << "##   #####   #   ####    ####   #####   ######  #####    ##          ####    ##\n"
    << "##   #    #  #  #       #    #  #    #  #         #     #  #        #    #   ##\n"
    << "##   #    #  #   ####   #       #    #  #####     #    #    #  ###  #    #   ##\n"
    << "##   #    #  #       #  #       #####   #         #    ######       #  # #   ##\n"
    << "##   #    #  #  #    #  #    #  #   #   #         #    #    #       #   ##   ##\n"
    << "##   #####   #   ####    ####   #    #  ######    #    #    #        #####   ##\n"
    << "##                                                                           ##\n"
    << "#############################################################################*/\n"
    << "\n"
    << "Syntax:\n"
    << "\n"
    << "discretaq -kramermesnermatrix -q [int] -t [int] -k [int] -group [string]\n"
    << "\n"
    << "discretaq -blockingsetmatrix -q [int] -t [int] -group [string] \n"
    << "\n"
    << "discretaq -linearcodes -q [int] -n [int] \n"
    << "\n"
    << "discretaq -setgroup -q [int] -n [int] -group [string] \n"
    << "\n"
    << "------------------------------------------------------------------------------\n"
    << "\n"
    << "The argument [string] for the group has the following structure\n"
    << "\n"
    << "(1) UNARY OPERATION: [name] [int]\n"
    << "\n"
    << "  [name] can be ...\n"
    << "\n"
    << "  one               > the one subgroup\n"
    << "  monomial          > the complete monomial group M(n,q)\n"
    << "  symmetric         > the group of permutation matrices corresponding to S_n\n"
    << "  cyclicperm        > the group of permutation matrices corresponding to C_n\n"
    << "  borel             > the group of upper triangular matrices B(n,q)\n"
    << "  singer            > the Singer cycle SC(n,q)\n"
    << "  normalizersinger  > the normalizer of the Singer cycle in GL(n,q)\n"
    << "  frobenius         > the group generated by the frobenius automorphism F(n,q)\n"
    << "\n"
    << "  Example:\n"
    << "\n"
    << "  discretaq -kramermesnermatrix -q 2 -t 3 -k 4 -group normalizersinger 8\n"
    << "\n"
    << "  computes the Kramer Mesner matrix M_{3,4}^G where G is the normalizer of the\n"
    << "  Singer cycle of GL(8,2).\n"
    << "\n"
    << "\n"
    << "(2) DIRECT PRODUCT: [string1] [string2] x\n"
    << "\n"
    << "                     > the direct product of the group G1 given by [string1]\n"
    << "                     > and the group G2 given by [string2]\n"
    << "\n"
    << "  Example:\n"
    << "\n"
    << "  discretaq -kramermesnermatrix -q 2 -t 2 -k 3 -group singer 4 monomial 3 x\n"
    << "\n"
    << "  computes the Kramer Mesner matrix M_{3,4}^G where G is a subgroup of GL(7,3)\n"
    << "  the direct product SC(4,2) x M(3,2).\n"
    << "\n"
    << "------------------------------------------------------------------------------\n";
    exit(1);
  }


 /*--------------------------
    kramermesnermatrix
--------------------------*/

  else if (strcmp(argv[1],"-setgroup") == 0)
  {

/*--------------------------
           help
--------------------------*/

    if (argc == 2)
    {
      cout << "\nSyntax:\n\n";
      cout << "discretaq -setgroup -q [int] ";
      cout << "-group [string] \n\n";
      exit(1);
    }

    for (int m = 1; m < argc; m++)
    {

/*--------------------------
        setting q
--------------------------*/

      if ( strcmp(argv[m],"-q") == 0 )
      {
        if (m+1 == argc)
        {
          cout << "ERROR: what is q?\n";
          exit(1);
        }
        q = atoi(argv[m+1]);
        if (q <= 1)
        {
          cout << "ERROR: what is q?\n";
          exit(1);
        }
        set_field(q);
        cout << "q = " << q << endl;
      }

    }  // end: for (int m = 1; m < argc; m++) { ...

/*--------------------------
        setting group
--------------------------*/

    for (int m = 1; m < argc; m++)
    {
      if ( strcmp(argv[m],"-group") == 0 )
      { 
        char groupname[256] = "";
        vector<matrix_group> stack;
        int sc = -1;    // stack counter
        if (m == argc-2)
        {
          cout << "ERROR: group - unknown list of arguments\n";
          exit(1);
        }
        for (int i = m+1; i < argc ; i++)
        {
          if (strcmp(argv[i],"file") == 0)
          {
            char nameinfile[256]= "";
            ifstream f(argv[i+1]);
            if (!f)
            {
              cout << "ERROR: file " << argv[i+1] << " does not exist\n";
              exit(1);
            }
            f >> nameinfile;
            f >> n;
            int nog;   // number of generators
            f >> nog;
            vector<matrix<gfq> > Gen;
            for (int l = 0; l < nog; l++)
            {
	      int tmp;
              matrix<gfq> tmp_M(n,n);
              for (int o = 0; o < n; o++)
                for (int j = 0; j < n; j++)
                {
                  f >> tmp;
                  tmp_M[o][j].init(tmp);
                }
              f >> tmp;
	      tmp_M.power_n(tmp);
	      cout << tmp_M;
              Gen.push_back(tmp_M);
            }
            f.close();
            if (nog == 1) G.cyclic(Gen[0]);
            else G.generate(Gen);
            stack.push_back(G);
            sc++;
            strcat(group,nameinfile);
            strcat(groupname,"file ");
	    strcat(groupname,argv[i+1]);
	    strcat(groupname," ");
          }
          if (arg_is_well_known_group(argv[i]))
          {
            if (i+1 >= argc )
            {
              cout << "ERROR: what is n?\n";
              exit(1);
            }
            n = atoi(argv[i+1]);
            if (n <= 0)
            {
              cout << "ERROR: what is n?\n";
              exit(1);
            }
            G.init_well_known_group(argv[i],n);
            stack.push_back(G);
            sc++;
            strcat(group,argv[i]);
            strcat(group,argv[i+1]);
            strcat(groupname,argv[i]);strcat(groupname," ");
	    strcat(groupname,argv[i+1]);strcat(groupname," ");

          }
          if (arg_is_binop(argv[i]))
          {
            if (sc == 0)
            {
              cout << "ERROR: too few arguments for binary operation\n";
              exit(1);
            }
            G.init_binop(argv[i],stack[sc],stack[sc-1]);
            stack.pop_back();
            sc--;
            stack[sc] = G;
            strcat(group,argv[i]);
            strcat(groupname,argv[i]);strcat(groupname," ");
          }
        }
        if (sc != 0)
        {
          cout << "ERROR: group - unknown list of arguments\n";
          exit(1);
        }
        else G = stack[0];
        n = G.n();
        cout << "n = " << n << endl;
        cout << "group = " << group << endl;
        cout << "group order = " << G.group_order() << endl;
        cout << "group memory = " << G.memory() << endl;
        ofstream groupinfofile("group_info.txt");
	groupinfofile << groupname << endl;
	groupinfofile << group << endl;
	groupinfofile << n << endl;
	groupinfofile << q << endl;
	groupinfofile << G.group_order() << endl;
        groupinfofile.close();
        // speichere informationen in datei  "group_info.txt" ab!!!

      }   // end: if (strcmp(argv[m],"-group") == 0 ) { ...


    }  // end: for (int m = 1; m < argc; m++) { ...

  }     // end: else if (strcmp(argv[1],"-setgroup") == 0) { ...












/*--------------------------
    kramermesnermatrix
--------------------------*/

  else if (strcmp(argv[1],"-kramermesnermatrix") == 0)
  {

/*--------------------------
           help
--------------------------*/

    if (argc == 2)
    {
      cout << "\nSyntax:\n\n";
      cout << "discretaq -kramermesnermatrix -q [int] ";
      cout << "-t [int] -k [int] -group [string] \n\n";
      exit(1);
    }

    for (int m = 1; m < argc; m++)
    {

/*--------------------------
         verbose
--------------------------*/

      if ( strcmp(argv[m],"-v") == 0 ) verbose = true;

/*--------------------------
        setting q
--------------------------*/

      if ( strcmp(argv[m],"-q") == 0 )
      {
        if (m+1 == argc)
        {
          cout << "ERROR: what is q?\n";
          exit(1);
        }
        q = atoi(argv[m+1]);
        if (q <= 1)
        {
          cout << "ERROR: what is q?\n";
          exit(1);
        }
        set_field(q);
        cout << "q = " << q << endl;
      }

/*--------------------------
        setting t
--------------------------*/

      if ( strcmp(argv[m],"-t") == 0 )
      {
        if (m+1 == argc)
        {
          cout << "ERROR: what is t?\n";
          exit(1);
        }
        t = atoi(argv[m+1]);
        if (t <= 0)
        {
          cout << "ERROR: what is t?\n";
          exit(1);
        }
        cout << "t = " << t << endl;
      }

/*--------------------------
        setting k
--------------------------*/

      if ( strcmp(argv[m],"-k") == 0 )
      {
        if (m+1 == argc)
        {
          cout << "ERROR: what is k?\n";
          exit(1);
        }
        k = atoi(argv[m+1]);
        if (k <= 0)
        {
          cout << "ERROR: what is k?\n";
          exit(1);
        }
        cout << "k = " << k << endl;
      }

    }  // end: for (int m = 1; m < argc; m++) { ...

/*--------------------------
        setting group
--------------------------*/

    for (int m = 1; m < argc; m++)
    {
      if ( strcmp(argv[m],"-group") == 0 )
      {
        vector<matrix_group> stack;
        int sc = -1;    // stack counter
        if (m == argc-2)
        {
          cout << "ERROR: group - unknown list of arguments\n";
          exit(1);
        }
        for (int i = m+1; i < argc ; i++)
        {
          if (strcmp(argv[i],"file") == 0)
          {
            char groupname[256]= "";
            ifstream f(argv[i+1]);
            if (!f)
            {
              cout << "ERROR: file " << argv[i+1] << " does not exist\n";
              exit(1);
            }
            f >> groupname;
            f >> n;
            int nog;   // number of generators
            f >> nog;
            vector<matrix<gfq> > Gen;
            for (int l = 0; l < nog; l++)
            {
	      int tmp;
              matrix<gfq> tmp_M(n,n);
              for (int o = 0; o < n; o++)
                for (int j = 0; j < n; j++)
                {
                  f >> tmp;
                  tmp_M[o][j].init(tmp);
                }
              f >> tmp;
	      tmp_M.power_n(tmp);
	      cout << tmp_M;
              Gen.push_back(tmp_M);
            }
            f.close();
            if (nog == 1) G.cyclic(Gen[0]);
            else G.generate(Gen);
            stack.push_back(G);
            sc++;
            strcat(group,groupname);
          }
          if (arg_is_well_known_group(argv[i]))
          {
            if (i+1 >= argc )
            {
              cout << "ERROR: what is n?\n";
              exit(1);
            }
            n = atoi(argv[i+1]);
            if (n <= 0)
            {
              cout << "ERROR: what is n?\n";
              exit(1);
            }
            G.init_well_known_group(argv[i],n);
            stack.push_back(G);
            sc++;
            strcat(group,argv[i]);
            strcat(group,argv[i+1]);
          }
          if (arg_is_binop(argv[i]))
          {
            if (sc == 0)
            {
              cout << "ERROR: too few arguments for binary operation\n";
              exit(1);
            }
            G.init_binop(argv[i],stack[sc],stack[sc-1]);
            stack.pop_back();
            sc--;
            stack[sc] = G;
            strcat(group,argv[i]);
          }
        }
        if (sc != 0)
        {
          cout << "ERROR: group - unknown list of arguments\n";
          exit(1);
        }
        else G = stack[0];
        n = G.n();
        cout << "n = " << n << endl;
        cout << "group = " << group << endl;
        cout << "group order = " << G.group_order() << endl;
        cout << "group memory = " << G.memory() << endl;
      }   // end: if (strcmp(argv[m],"-group") == 0 ) { ...

    }  // end: for (int m = 1; m < argc; m++) { ...


/*--------------------------
      testing t and k
--------------------------*/

    if ( t <= 0 || t >= k || k >= n )
    {
      cout << "ERROR: parameters t and k do not fulfill 0 < t < k < n!\n";
      exit(1);
    }

/*--------------------------
      orbit algorithm
--------------------------*/

    cout << "computing the orbits of G on the t- and k-subspaces\n";
    compute_orbits(n-k,G,D,history_rep,stab_orders,verbose);

/*--------------------------
 incidence matrices M^G_{i,j}
 1 <= i < j <= k
--------------------------*/

    cout << "calculating the incidence matrices\n";
    kmmttp1 = all_Mttp1(n,n-k,history_rep,stab_orders);
    for (int i = 1; i <= k; i++)
    {
      for (int j = i+1; j <= k; j++)
      {
        save_ASCII_A_inf_from_dual(n,i,j,group,stab_orders,kmmttp1);
        save_LaTeX_A_inf_from_dual(n,i,j,group,stab_orders,kmmttp1);
      }
    }

/*--------------------------
 save orbits on n-i-subspaces
 1 <= i <= k
--------------------------*/

    cout << "saving orbits\n";
    for (int i = 1; i <= k; i++)
    {
      save_ASCII_orbits_from_dual(i,group,D,stab_orders);
      save_LaTeX_orbits_from_dual(i,group,D,stab_orders);
    }



  }     // end: else if (strcmp(argv[1],"-kramermesnermatrix") == 0) { ...


/*--------------------------
   blockingsetmatrix
--------------------------*/


  else if (strcmp(argv[1],"-blockingsetmatrix") == 0)
  {

/*--------------------------
           help
--------------------------*/

    if (argc == 2)
    {
      cout << "\nSyntax:\n\n";
      cout << "discretaq -blockingsetmatrix -q [int] ";
      cout << "-t [int] -group [string] [-B [int]] \n\n";
      exit(1);
    }

    for (int m = 1; m < argc; m++)
    {

/*--------------------------
         verbose
--------------------------*/

      if ( strcmp(argv[m],"-v") == 0 ) verbose = true;

/*--------------------------
        setting q
--------------------------*/

      if ( strcmp(argv[m],"-q") == 0 )
      {
        if (m+1 == argc)
        {
          cout << "ERROR: what is q?\n";
          exit(1);
        }
        q = atoi(argv[m+1]);
        if (q <= 1)
        {
          cout << "ERROR: what is q?\n";
          exit(1);
        }
        set_field(q);
        cout << "q = " << q << endl;
      }

/*--------------------------
        setting t
--------------------------*/

      if ( strcmp(argv[m],"-t") == 0 )
      {
        if (m+1 == argc)
        {
          cout << "ERROR: what is t?\n";
          exit(1);
        }
        t = atoi(argv[m+1]);
        if (t <= 0)
        {
          cout << "ERROR: what is t?\n";
          exit(1);
        }
        cout << "t = " << t << endl;
      }


    }  // end: for (int m = 1; m < argc; m++) { ...

/*--------------------------
        setting group
--------------------------*/

    for (int m = 1; m < argc; m++)
    {
      if ( strcmp(argv[m],"-group") == 0 )
      {
        vector<matrix_group> stack;
        int sc = -1;    // stack counter
        if (m == argc-2)
        {
          cout << "ERROR: group - unknown list of arguments\n";
          exit(1);
        }
        for (int i = m+1; i < argc ; i++)
        {
          if (strcmp(argv[i],"file") == 0)
          {
            char groupname[256]= "";
            ifstream f(argv[i+1]);
            if (!f)
            {
              cout << "ERROR: file " << argv[i+1] << " does not exist\n";
              exit(1);
            }
            f >> groupname;
            f >> n;
            int nog;   // number of generators
            f >> nog;
            vector<matrix<gfq> > Gen;
            for (int l = 0; l < nog; l++)
            {
	      int tmp;
              matrix<gfq> tmp_M(n,n);
              for (int o = 0; o < n; o++)
                for (int j = 0; j < n; j++)
                {
                  f >> tmp;
                  tmp_M[o][j].init(tmp);
                }
              f >> tmp;
	      tmp_M.power_n(tmp);
	      cout << tmp_M;
              Gen.push_back(tmp_M);
            }
            f.close();
            if (nog == 1) G.cyclic(Gen[0]);
            else G.generate(Gen);
            stack.push_back(G);
            sc++;
            strcat(group,groupname);
          }
          if (arg_is_well_known_group(argv[i]))
          {
            if (i+1 >= argc )
            {
              cout << "ERROR: what is n?\n";
              exit(1);
            }
            n = atoi(argv[i+1]);
            if (n <= 0)
            {
              cout << "ERROR: what is n?\n";
              exit(1);
            }
            G.init_well_known_group(argv[i],n);
            stack.push_back(G);
            sc++;
            strcat(group,argv[i]);
            strcat(group,argv[i+1]);
          }
          if (arg_is_binop(argv[i]))
          {
            if (sc == 0)
            {
              cout << "ERROR: too few arguments for binary operation\n";
              exit(1);
            }
            G.init_binop(argv[i],stack[sc],stack[sc-1]);
            stack.pop_back();
            sc--;
            stack[sc] = G;
            strcat(group,argv[i]);
          }
        }
        if (sc != 0)
        {
          cout << "ERROR: group - unknown list of arguments\n";
          exit(1);
        }
        else G = stack[0];
        n = G.n();
        cout << "n = " << n << endl;
        cout << "group = " << group << endl;
        cout << "group order = " << G.group_order() << endl;

      }   // end: if (strcmp(argv[m],"-group") == 0 ) { ...

    }  // end: for (int m = 1; m < argc; m++) { ...


/*--------------------------
        setting Bmax
--------------------------*/

    Bmax = gauss_nkq(n,1,q);
    cout << "Bmax = " << Bmax << endl;

/*--------------------------
      testing t and k
--------------------------*/

    if ( t <= 0 || t > (n/2) )
    {
      cout << "ERROR: parameter t does not fulfill 0 < t <= n/2!\n";
      exit(1);
    }

/*--------------------------
      orbit algorithm
--------------------------*/

    cout << "computing the orbits of G on the t- and (n-t)-subspaces\n";
    compute_orbits(t,G,D,history_rep,stab_orders,verbose);

/*--------------------------
 incidence matrices M^G_{i,j}
 1 <= i < j <= k
--------------------------*/

    cout << "calculating the incidence matrices\n";
    kmmttp1 = all_Mttp1(n,t,history_rep,stab_orders);
    save_ASCII_blockingsetmatrix(n,t,group,stab_orders,kmmttp1,Bmax);


 /*--------------------------
 save orbits on t-
 and (n-t)-subspaces
--------------------------*/

    cout << "saving orbits\n";
    save_ASCII_orbits(t,group,D,stab_orders);
    save_ASCII_orbits(n-t,group,D,stab_orders);



  }     // end: else if (strcmp(argv[1],"-blockingsetmatrix") == 0) { ...


/*--------------------------
    linearcodes
--------------------------*/

  else if (strcmp(argv[1],"-linearcodes") == 0)
  {


  /*--------------------------
           help
--------------------------*/

    if (argc == 2)
    {
      cout << "\nSyntax:\n\n";
      cout << "discretaq -linear codes -q [int] ";
      cout << "-n [int] \n\n";
      exit(1);
    }

    for (int m = 1; m < argc; m++)
    {

/*--------------------------
         verbose
--------------------------*/

      if ( strcmp(argv[m],"-v") == 0 ) verbose = true;

/*--------------------------
        setting q
--------------------------*/

      if ( strcmp(argv[m],"-q") == 0 )
      {
        if (m+1 == argc)
        {
          cout << "ERROR: what is q?\n";
          exit(1);
        }
        q = atoi(argv[m+1]);
        if (q <= 1)
        {
          cout << "ERROR: what is q?\n";
          exit(1);
        }
        set_field(q);
        cout << "q = " << q << endl;
      }

/*--------------------------
        setting n
--------------------------*/

      if ( strcmp(argv[m],"-n") == 0 )
      {
        if (m+1 == argc)
        {
          cout << "ERROR: what is n?\n";
          exit(1);
        }
        n = atoi(argv[m+1]);
        if (n <= 0)
        {
          cout << "ERROR: what is n?\n";
          exit(1);
        }
        cout << "n = " << n << endl;
      }


    }  // end: for (int m = 1; m < argc; m++) { ...


 /*--------------------------
        setting the group
--------------------------*/
    char *tmp;
    tmp = new char[2];
    gcvt(n,2,tmp);
    strcat(group,"monomial");
    strcat(group,tmp);
    delete [] tmp;
    G.monomial(n);
    cout << "group = " << group << endl;
    cout << "group order = " << G.group_order() << endl;
    cout << "group memory = " << G.memory() << endl;


 /*--------------------------
        setting the group
--------------------------*/
    if (n%2 == 0) t = n/2;
    else t = (n/2) +1;
    cout << "t = " << t << endl;
    cout << "computing the linear codes ...\n";
    compute_only_orbits(t,G,D,history_rep,verbose);


  }     // end: else if (strcmp(argv[1],"-linearcodes") == 0) { ...



/*--------------------------
    frobeniusmatrix
--------------------------*/

  else if (strcmp(argv[1],"-frobeniusmatrix") == 0)
  {


  /*--------------------------
           help
--------------------------*/

    if (argc == 2)
    {
      cout << "\nSyntax:\n\n";
      cout << "discretaq -frobeniusmatrix -q [int] -n [int] \n\n";
      exit(1);
    }

    for (int m = 1; m < argc; m++)
    {

/*--------------------------
        setting q
--------------------------*/

      if ( strcmp(argv[m],"-q") == 0 )
      {
        if (m+1 == argc)
        {
          cout << "ERROR: what is q?\n";
          exit(1);
        }
        q = atoi(argv[m+1]);
        if (q <= 1)
        {
          cout << "ERROR: what is q?\n";
          exit(1);
        }
        set_field(q);
        cout << "q = " << q << endl;
      }

/*--------------------------
        setting n
--------------------------*/

      if ( strcmp(argv[m],"-n") == 0 )
      {
        if (m+1 == argc)
        {
          cout << "ERROR: what is n?\n";
          exit(1);
        }
        n = atoi(argv[m+1]);
        if (n <= 0)
        {
          cout << "ERROR: what is n?\n";
          exit(1);
        }
        cout << "n = " << n << endl;
      }


    }  // end: for (int m = 1; m < argc; m++) { ...

/*--------------------------
        setting n
--------------------------*/
   cout << frobenius_aut_matrix(n) << endl;




  }     // end: else if (strcmp(argv[1],"-frobeniusmatrix") == 0) { ...


/*--------------------------
    frobeniusmatrix
--------------------------*/

  else if (strcmp(argv[1],"-singermatrix") == 0)
  {


  /*--------------------------
           help
--------------------------*/

    if (argc == 2)
    {
      cout << "\nSyntax:\n\n";
      cout << "discretaq -singermatrix -q [int] -n [int] \n\n";
      exit(1);
    }

    for (int m = 1; m < argc; m++)
    {

/*--------------------------
        setting q
--------------------------*/

      if ( strcmp(argv[m],"-q") == 0 )
      {
        if (m+1 == argc)
        {
          cout << "ERROR: what is q?\n";
          exit(1);
        }
        q = atoi(argv[m+1]);
        if (q <= 1)
        {
          cout << "ERROR: what is q?\n";
          exit(1);
        }
        set_field(q);
        cout << "q = " << q << endl;
      }

/*--------------------------
        setting n
--------------------------*/

      if ( strcmp(argv[m],"-n") == 0 )
      {
        if (m+1 == argc)
        {
          cout << "ERROR: what is n?\n";
          exit(1);
        }
        n = atoi(argv[m+1]);
        if (n <= 0)
        {
          cout << "ERROR: what is n?\n";
          exit(1);
        }
        cout << "n = " << n << endl;
      }


    }  // end: for (int m = 1; m < argc; m++) { ...

/*--------------------------
        setting n
--------------------------*/
   cout << singer_matrix(n) << endl;




  }     // end: else if (strcmp(argv[1],"-singermatrix") == 0) { ...



/*--------------------------
    else: help
--------------------------*/

  else
  {
    cout << "\nSyntax:\n\n";
    cout << "discretaq -kramermesnermatrix -q [int] ";
    cout << "-t [int] -k [int] -group [string]\n\n";
    cout << "discretaq -blockingsetmatrix -q [int] ";
    cout << "-t [int] -group [string] [-B [int]]\n\n";
    cout << "discretaq -linear codes -q [int] ";
    cout << "-n [int] \n\n";
    exit(1);
  }      // end: else

/*--------------------------
    end
--------------------------*/

  cout << "discretaq finished\n";
  return 0;

}









