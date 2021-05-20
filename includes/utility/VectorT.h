//
// Created by Saman Seifi Ph.D. on 5/15/21.
//

#ifndef SIMPLEBEAM_VECTORT_H
#define SIMPLEBEAM_VECTORT_H

/* base class */
#include "ArrayT.h"

/** template base class for array of number type (nTYPE)
 * with linear algebraic operations of vectors defined.
 * @tparam nTYPE 
 */
template <class nTYPE>
class VectorT: public ArrayT<nTYPE> {

public:
    /** constructors */
    /*@{*/
    VectorT();

    /** construct a vector of the specified length. The values in the array are not initialized.
    *   \param length length of dynamically allocated space */
    explicit VectorT(int length);

    /** construct an alias
    \* \param ptrMATHTYPE pointer to the memory to use */
    explicit VectorT(const nTYPE* ptrMATHTYPE);

    /** copy constructor */
    VectorT(const VectorT& source);
    /*@{*/

    /** \name Assignment operators */
    /*@{*/
    /** Set all elements in the array to the given different data types */
    VectorT<nTYPE>& operator=(const nTYPE& valueRHS);
    VectorT<nTYPE>& operator=(const nTYPE* arrRHS);
    VectorT<nTYPE>& operator=(const VectorT<nTYPE>& vecRHS);
    /*@{*/

    /** \name addition operators */
    /*@{*/
    VectorT<nTYPE>& operator+=(const nTYPE& value); /**< add value to all elements */
//    VectorT<nTYPE>& operator+=(const nTYPE* pRHS);  /**< element-by-element addition with pRHS (without range checking). */
//    VectorT<nTYPE>& operator+=(const VectorT& RHS); /**< element-by-element addition with RHS */
    /*@}*/


    /* Adding element-by-element with given vector */
    void Add(VectorT& vec);

    /* Adding a hopefully smaller array to the base array with a starting row number */
    void AddArray(VectorT& arr, int row);

    /* Scaling the vector */
    void SetToScaled(const nTYPE& scale);

    /* Printing out the element-by-element */
    virtual void Print() {
        for (int i = 0; i < this->fLength; i++) {
            cout << this->fArray[i] << endl;
        }
    }
};

/* Constructors */
template<class nTYPE>
VectorT<nTYPE>::VectorT() = default;

template <class nTYPE>
inline VectorT<nTYPE>::VectorT(int length): ArrayT<nTYPE>(length) { }

template <class nTYPE>
inline VectorT<nTYPE>::VectorT(const nTYPE* ptrMATHTYPE): ArrayT<nTYPE>(ptrMATHTYPE) { }

template <class nTYPE>
inline VectorT<nTYPE>::VectorT(const VectorT& source): ArrayT<nTYPE>(source) { }

/* Operators definitions */
template<class nTYPE>
inline VectorT<nTYPE>& VectorT<nTYPE>::operator=(const nTYPE& valueRHS) {

    /* Inherited */
    ArrayT<nTYPE>::operator=(valueRHS);
    return *this;
}

template<class nTYPE>
inline VectorT<nTYPE>& VectorT<nTYPE>::operator=(const nTYPE* arrRHS) {

    /* First finding the length of given array */
    int arrayLength = (sizeof(*arrRHS) / sizeof(arrRHS)) + 1;    // proper way of getting the length of an array

    /* Check the length */
    assert(this->fLength == arrayLength);

    for (int i = 0; i < arrayLength; i++){
        this->fArray[i] = arrRHS[i];
    }

}

template<class nTYPE>
inline VectorT<nTYPE> &VectorT<nTYPE>::operator=(const VectorT<nTYPE> &vecRHS) {

    /* Inherited */
    ArrayT<nTYPE>::operator=(vecRHS);

    return *this;
}

template<class nTYPE>
VectorT<nTYPE>& VectorT<nTYPE>::operator+=(const nTYPE& valueRHS) {

    nTYPE* ptrTemp_ = this->Pointer();
    for (int i = 0; i < this->fLength; i++){
        *ptrTemp_++ += valueRHS;
    }
    return *this;
}

//template<class nTYPE>
//VectorT<nTYPE>& VectorT<nTYPE>::operator+=(const nTYPE* arrRHS) {
//    return <#initializer#>;
//}
//
//template<class nTYPE>
//VectorT<nTYPE>& VectorT<nTYPE>::operator+=(const VectorT& vRHS) {
//    return <#initializer#>;
//}

/* Scaling all of the elements to a given scale value */
template <class nTYPE>
void VectorT<nTYPE>::SetToScaled(const nTYPE& scale) {

    for (int i = 0; i < this->fLength; i++) {
        this->fArray[i] *= scale;
    }
}

/* Adding a sub vector from a offset row number */
template <class nTYPE>
void VectorT<nTYPE>::AddArray(VectorT &arr, int row) {

    /* Check the length doesn't exceed the original vector */
    assert(arr.Length() < this->fLength - row);

    int j = 0;
    for (int i = row; i < arr.Length(); i++) {
        this->fArray[i] += arr[j];
        j++;
    }
}

template <class nTYPE>
void VectorT<nTYPE>::Add(VectorT<nTYPE>& vec) {

    /* Check the length */
    assert(this->fLength == vec.Length());

    for (int i = 0; i < this->fLength; i++) {
        this->fArray[i] += vec[i];
    }
}

#endif //SIMPLEBEAM_VECTORT_H