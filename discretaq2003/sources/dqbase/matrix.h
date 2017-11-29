#ifndef __MATRIX_H
#define __MATRIX_H

// Michael Braun
// 2003.01.27
// matrix.h

#include <iostream>
#include <vector>


using namespace std;

/*#############################################################################
##                                                                           ##
##                               MATRIX                                      ##
##                                                                           ##
#############################################################################*/

template<class T>
class matrix: public vector<vector<T> >
{


  public:


/*****************************************************************************/
    matrix(){}   // Standardconstructor

/*****************************************************************************/
    matrix(
            size_t x,                       // number of rows x
            size_t y                        // number of columns y
    ):vector<vector<T> >(x, vector<T>(y)){} // return: empty x times y matrix
                                            // with entries of type T

/*****************************************************************************/
    size_t rows() const;   // return: number of rows of matrix this

/*****************************************************************************/
    size_t columns() const;  // return: number of columns of matrix this

/*****************************************************************************/
    matrix<T>& init(const T& value); // return: initialises each entry with
                                     // "value"

/*****************************************************************************/
    matrix<T>& I(); // return: this = unit matrix

/*****************************************************************************/
    matrix<T>& revers_I(); // return:   0 ... 0 1
                           //           .     . 0
                           //   this =  .   .   .
                           //           0 .     .
                           //           1 0 ... 0

/*****************************************************************************/
    matrix<T>& O();  // return: this = zero matrix

/*****************************************************************************/
    matrix<T>& permut_rows(
            size_t i,         // row index i
            size_t j          // row index j
    );                        // return: permutes the rows i and j of this

/*****************************************************************************/
    matrix<T>& permut_columns(
            size_t i,         // column index i
            size_t j          // column index j
    );                        // permutes the columns i and j

/*****************************************************************************/
    matrix<T>& add_vector(
           const vector<T>& v      // vector v
    );                             // return: matrix [this|v]

/*****************************************************************************/
    matrix<T>& transpose(); // return : this = this^t

/*****************************************************************************/
    matrix<T>& invert();    // return : this = this^{-1}

/*****************************************************************************/
    matrix<T> operator+(
            const matrix<T>& M    // matrix M
    ) const;                      // return: this + M

/*****************************************************************************/
    matrix<T>& operator+=(
            const matrix<T>& M   // return M
    );                           // return: this = this + M

/*****************************************************************************/
    matrix<T> operator-();       // return: this = -this

/*****************************************************************************/
    matrix<T> operator-(
            const matrix<T>& M    // matrix M
    ) const;                      // return: this - M

/*****************************************************************************/
    matrix<T>& operator-=(
            const matrix<T>& M    // matrix M
    );                            // return: this = this - M

/*****************************************************************************/
    matrix<T> operator*(
            const matrix<T>& M    // matrix M
    ) const;                      // return: this * M

/*****************************************************************************/
    matrix<T>& operator*=(
            const matrix<T>& M    // matrix M
    );                            // return: this = this * M

/****************************************************************************/
    matrix<T>& power_n(
            int n         // natural number n
    );                    // return: this = this^n

};


/*****************************************************************************/
// out stream for matrix
template<class T>
ostream& operator<<(ostream& s, const matrix<T>& M);


/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/
/*****************************************************************************/


template<class T>
    size_t matrix<T>::rows() const
    {
      return size();
    }

template<class T>
    size_t matrix<T>::columns() const
    {
      if (rows() == 0) return 0;
      else return (*this)[0].size();
    }

template<class T>
    matrix<T>& matrix<T>::init(const T& value)
    {
      size_t i, j;
      for (i = 0; i < rows(); i++)
        for (j = 0; j < columns(); j++)
          (*this)[i][j] = value;
      return *this;
    }

template<class T>
    matrix<T>& matrix<T>::I()
    {
      size_t i, j;
      for (i = 0; i < rows(); i++)
        for (j = 0; j < columns(); j++)
          (*this)[i][j] = (i==j) ? T(1) : T(0);
      return *this;
    }

template<class T>
    matrix<T>& matrix<T>::revers_I()
    {
      for (size_t i = 0; i < rows(); i++)
        for (size_t j = 0; j < columns(); j++)
          (*this)[i][columns()-1-j] = (i==j) ? T(1) : T(0);
      return *this;
    }

template<class T>
    matrix<T>& matrix<T>::O()
    {
      for (size_t i = 0; i < rows(); i++)
        for (size_t j = 0; j < columns(); j++)
          (*this)[i][j] = T(0);
      return *this;
    }


template<class T>
    matrix<T>& matrix<T>::permut_rows(size_t i, size_t j)      
    {
      if (i >= rows() || j >= rows())
      {
        cout << "ERROR: matrix::permut_rows(i,j): i or j too big!\n";
        exit(1);
      }
      T v;
      for (size_t k = 0; k < columns(); k++)
      {
        v = (*this)[i][k];
        (*this)[i][k] = (*this)[j][k];
        (*this)[j][k] = v;
      }
      return *this;
    }

template<class T>
    matrix<T>& matrix<T>::permut_columns(size_t i, size_t j)
    {
      if (i >= columns() || j >= columns())
      {
        cout << "ERROR: matrix::permut_columns(i,j): i or j too big!\n";
        exit(1);
      }
      T v;
      for (size_t k = 0; k < rows(); k++)
      {
        v = (*this)[k][i];
        (*this)[k][i] = (*this)[k][j];
        (*this)[k][j] = v;
      }
      return *this;
    }

template<class T>
    matrix<T>& matrix<T>::add_vector(const vector<T>& v)   
    {
      if (rows() != v.size())
      {
        cout << "ERROR: matrix::add_vector: matrix and vector "
	     << "are not compartible!\n";
        exit(1);
      }
      matrix<T> Temp(rows(),columns()+1);
      for (size_t i = 0; i < rows(); i++)
      {
        for (size_t j = 0; j < columns(); j++) Temp[i][j] = (*this)[i][j];
        Temp[i][columns()] = v[i];
      }
      (*this) = Temp;
      return (*this);
    }

template<class T>
    matrix<T>& matrix<T>::transpose()
    {
      matrix<T> Temp(columns(),rows());
      for (size_t i = 0; i < Temp.rows(); i++)
        for (size_t j = 0; j < Temp.columns(); j++)
          Temp[i][j] = (*this)[j][i];
      (*this) = Temp;
      return *this;
    }

template<class T>
    matrix<T>& matrix<T>::invert()
    {
      short t;
      matrix<T> Temp(rows(), columns());
      size_t i, j, k;
      Temp.I();
      T v;
      vector<short> p(rows());
      for (i = 0; i < rows(); i++)
        p[i] = i;
      for (j = 0; j < columns(); j++)
      {
        for (i = j; i < rows(); i++)
          if (i == rows()-1 || (*this)[p[i]][j] != T(0))
            break;
        if ((*this)[p[i]][j] == T(0))
        {
          cout << "ERROR: matrix::invert(): matrix ist not invertable!\n";
          exit(1);
        }
        t = p[i];
        p[i] = p[j];
        p[j] = t;
        v = (*this)[p[j]][j];
        for (k = 0; k < columns(); k++)
        {
          (*this)[p[j]][k] /= v;
          Temp[p[j]][k] /= v;
        }
        for (i = 0; i < rows(); i++)
          if (i != j)
            if ((*this)[p[i]][j] != T(0))
            {
              v = (*this)[p[i]][j];
              for (k = 0; k < columns(); k++)
              {
                 Temp[p[i]][k] = Temp[p[i]][k] - Temp[p[j]][k]*v;
                 (*this)[p[i]][k] = (*this)[p[i]][k] - (*this)[p[j]][k]*v;
              }
            }
      }
      for (i = 0; i < rows(); i++)
        for (j = 0; j < columns(); j++)
          (*this)[i][j] = Temp[p[i]][j];
      return *this;
    }

template<class T>
    matrix<T> matrix<T>::operator+(const matrix<T>& M) const
    {
      if (rows() != M.rows() || columns() != M.columns)
      {
        cout << "ERROR: matrix::operator+(M): matrices are not addable!\n";
        exit(1);
      }
      matrix<T> sum(M.rows(),M.columns());
      for (size_t i = 0; i < M.rows(); i++)
        for (size_t j = 0; j < M.columns(); j++)
          sum[i][j] = (*this)[i][j] + M[i][j];
      return sum;
    }

template<class T>
    matrix<T>& matrix<T>::operator+=(const matrix<T>& M)
    {
      (*this) = (*this) + M;
      return *this;
    }


template<class T>
    matrix<T> matrix<T>::operator-()
    {
      matrix<T> Temp(rows(),columns());
      for (size_t i = 0; i < rows(); i++)
        for (size_t j = 0; j < columns(); j++)
          Temp[i][j] = -(*this)[i][j];
      return Temp;
    }

template<class T>
    matrix<T> matrix<T>::operator-(const matrix<T>& M) const
    {
      if (rows() != M.rows() || columns() != M.columns)
      {
        cout << "ERROR: matrix::operator-(M): matrices are not compartible!\n";
        exit(1);
      }
      matrix<T> sum(M.rows(),M.columns());
      for (size_t i = 0; i < M.rows(); i++)
        for (size_t j = 0; j < M.columns(); j++)
          sum[i][j] = (*this)[i][j] - M[i][j];
      return sum;
    }

template<class T>
    matrix<T>& matrix<T>::operator-=(const matrix<T>& M)
    {
      (*this) = (*this) - M;
      return *this;
    }

template<class T>
    matrix<T> matrix<T>::operator*(const matrix<T>& M) const
    {
      matrix<T> Temp(rows(),M.columns());
      size_t i, j, k;
      for (i = 0; i < Temp.rows(); i++)
        for (j = 0; j < Temp.columns(); j++)
        {
          Temp[i][j] = T(0);
          for (k = 0; k < columns(); k++)
            Temp[i][j] = Temp[i][j] + (*this)[i][k] * M[k][j];
        }
      return Temp;
    }

template<class T>
    matrix<T>& matrix<T>::operator*=(const matrix<T>& M)
    {
      (*this) = (*this) * M;
      return *this;
    }

template<class T>
    matrix<T>& matrix<T>::power_n(int n)                    
    {
      matrix<T> M = (*this), zero;
      zero.O();
      int i = n-1;
      if ((*this) != zero)
      {
        while (i > 0)
        {
          if ( (i%2) == 0 )
          {
            M *= M;
            i >>= 1;
          }
          (*this) *= M;
          i--;
        }
      }
      return *this;
    }


template<class T>
    ostream& operator<<(ostream& s, const matrix<T>& M)
    {
      s << endl;
      for (size_t i = 0; i < M.rows(); i++)
      {
        for (size_t j = 0; j < M.columns(); j++)
        s << M[i][j] << " ";
        s << endl;
      }
      return s;
    }


#endif



