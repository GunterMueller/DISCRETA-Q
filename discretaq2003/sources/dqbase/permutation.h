#ifndef __PERMUTATION_H
#define __PERMUTATION_H


// Michael Braun
// 2003.01.15
// permuation.h


#include <iostream>
#include <vector>


using namespace std;


/*#############################################################################
##                                                                           ##
##                               PERMUTATION                                 ##
##                                                                           ##
#############################################################################*/


class permutation: public vector<unsigned int>
{


  public:


    permutation() { }
        // Standardkonstruktor


    permutation(size_t n) : vector<unsigned int>(n) { }
        // legt eine leere Permutation vom Grade n an


    permutation(const vector<unsigned  int>& v);
        // initialisiert eine Permutation in Listenschreibweise
        // die Liste ist im Vektor "v" gespeichert
   
    
    permutation(const vector<int>& v);
        // initialisiert eine Permutation in Listenschreibweise
        // die Liste ist im Vektor "v" gespeichert


    permutation& list(const vector<int>& v);
        // initialisiert eine Permutation in Listenschreibweise
        // die Liste ist im Vektor "v" gespeichert


    permutation& list(const vector<unsigned int>& v);
        // initialisiert eine Permutation in Listenschreibweise
        // die Liste ist im Vektor "v" gespeichert	


    size_t degree() const;
        // gibt den Grad der Permutation "this" zurueck


    permutation& id(size_t n);
        // uberschreibt "this" mit dem Einselement von Grade "n"


    permutation& id();
        // uberschreibt "this" mit dem Einselement. Der Grad der
        // Permutation wird erhalten


    permutation operator*(const permutation& p) const;
        // multipliziert "this" mit "p" und gibt das Ergebnisprodukt
        // zurueck


    permutation& operator*=(const permutation& p);
        // multipliziert "this" mit "p" und ueberschreibt "this"
        // mit dem Produkt


    permutation& invert();
        // invertiert die Permutation "this" und ueberschreibt
        // "this" mit dem Inversen


};


ostream& operator<<(ostream& s, const permutation& p);
    // schreibt die Permutation in Zykelschreibweise auf den 
    // Ausgabestrom "s"


#endif

