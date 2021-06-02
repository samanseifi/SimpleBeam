//
// Created by Saman Seifi Ph.D. on 5/11/21.
//

#ifndef SIMPLEBEAM_MATRIXT_H
#define SIMPLEBEAM_MATRIXT_H

/* base class */
#include "VectorT.h"

/**
 * A two-dimensional mathematical matrix template object with number type (nTYPE)
 * The matrix object is vector in reality and the matrix multiplications are
 * implemented using vector elemental indexes. For example:
 */
template <class nTYPE>
class MatrixT: public VectorT<nTYPE> {

protected:

    /** \name dimensions */
    /*@{*/
    int fRows;    /**< Number of rows in the matrix */
    int fCols;    /**< Number of columns in the matrix */
    /*@}*/

public:

    /** Constructors */
    /*@{*/
    /* Default constructor */
    MatrixT();

    /* Create a matrix of rows=cols  */
    explicit MatrixT(int size);

    /* Create a matrix of rows and cols */
    explicit MatrixT(int rows, int cols);
    /*@}*/

    /** \name Operators */
    /* Accessors */
    /*@{*/
    /* Return a the (n_row, n_col) element in a matrix */
    nTYPE& operator()(int n_row, int n_col);
    const nTYPE& operator()(int n_row, int n_col) const;

    /* return a pointer a column in the matrix */
    nTYPE* operator()(int ncol);
    const nTYPE* operator()(int ncol) const;

    /* Assignments */
    MatrixT<nTYPE>& operator=(const nTYPE& valRHS);
    MatrixT<nTYPE>& operator=(const MatrixT<nTYPE>& matRHS);
    /*@}*/

    /** Transpose of the Matrix */
    MatrixT<nTYPE>& Transpose(const MatrixT<nTYPE>& matrix);

    /** Matrix inversion */

    /** Set the dimension of the square matrix */
    void Dimension(int squareMatSize);

    /** Set the dimension matrix */
    void Dimension(int numRows, int numCols);

    /** Get number of rows */
    int Rows() const {
        return fRows;
    };

    /** Get number of cols */
    int Cols() const {
        return fCols;
    };

    /** Printing the matrix */
    void Print() {

        for (int i = 0; i < fRows; i++) {
            for (int j = 0; j < fCols; j++) {
                cout << operator()(i, j) << " ";
            }
            cout << "\n";
        }
    };

};

// Constructors
template<class nTYPE>
MatrixT<nTYPE>::MatrixT(): fRows(0), fCols(0) { }

template<class nTYPE>
MatrixT<nTYPE>::MatrixT(int size): fRows(0), fCols(0) {

    Dimension(size);
}

template<class nTYPE>
MatrixT<nTYPE>::MatrixT(int rows, int cols): fRows(0), fCols(0) {

    Dimension(rows, cols);
}

// Setting the dimensions
template<class nTYPE>
void MatrixT<nTYPE>::Dimension(int squareMatSize) {

    Dimension(squareMatSize, squareMatSize);
}

template<class nTYPE>
void MatrixT<nTYPE>::Dimension(int numRows, int numCols) {

    fCols = fRows = 0;

    /* Update the length of this matrix's underlying array */
    VectorT<nTYPE>::Dimension(numRows * numCols);

    /* Update matrix dimensions */
    fRows = numRows;
    fCols = numCols;
}

// Operators
template<class nTYPE>
inline nTYPE& MatrixT<nTYPE>::operator()(int n_row, int n_col) {

    /* Checking the range */
    assert(n_row <= fRows && n_col <= fCols);

    /* From mapping the array with size of n_col+c_row to a matrix of n_row x n_col */
    /* (i, j) in matrix is (j*numRows + i) */
    return (this->fArray[n_col*fRows + n_row]);
}

template<class nTYPE>
inline const nTYPE& MatrixT<nTYPE>::operator()(int n_row, int n_col) const {

    /* Checking the range */
    assert(n_row <= fRows && n_col <= fCols);

    /**
     * From mapping the array with size of n_col+c_row to a matrix of n_row x n_col
     * (i, j) in matrix is (j*numRows + i)
     */
    return (this->fArray[n_col*fRows + n_row]);
}

/* returns a pointer to the top of the specified column */
template<class nTYPE>
inline nTYPE* MatrixT<nTYPE>::operator()(int ncol) {

    return(this->fArray + ncol*fRows);
}

template<class nTYPE>
inline const nTYPE* MatrixT<nTYPE>::operator()(int ncol) const {

    return(this->fArray + ncol*fRows);
}

template<class nTYPE>
inline MatrixT<nTYPE>& MatrixT<nTYPE>::operator=(const nTYPE& valRHS) {

    /* Inherited */
    VectorT<nTYPE>::operator=(valRHS);

    return (*this);
}

template<class nTYPE>
inline MatrixT<nTYPE>& MatrixT<nTYPE>::operator=(const MatrixT<nTYPE>& matRHS) {

    /* Inherited */
    MatrixT<nTYPE>::operator=(matRHS);

    /* Setting dimensions */
    fRows = matRHS.fRows;
    fCols = matRHS.fCols;

    return (*this);
}

template<class nTYPE>
MatrixT<nTYPE> &MatrixT<nTYPE>::Transpose(const MatrixT<nTYPE> &matrix) {

    nTYPE *pThis = this->fArray;
    nTYPE *pMat  = matrix.fArray;
    for (int i = 0; i < matrix.fRows; i++)
    {
        nTYPE* pmj = pMat++;
        for (int j = 0; j < matrix.fCols; j++)
        {
            *pThis++ = *pmj;
            pmj += matrix.fRows;
        }
    }
    return (*this);

}

#endif //SIMPLEBEAM_MATRIXT_H
