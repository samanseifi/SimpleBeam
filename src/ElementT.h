//
// Created by saman on 5/17/21.
//

#ifndef SIMPLEBEAM_ELEMENTT_H
#define SIMPLEBEAM_ELEMENTT_H

#include "ArrayT.h"

/**
 * A simple node object storing ID and its coordinate (1D for now)
 */
struct Node {
    int ID;             /***< node's ID */
    int fNodalNumDOFs;   /***< degree of freedom at each node */

    double* DOFs;       /***< Main holder of dofs */

    /** Default Constructor */
    Node() {
        ID = 0;
        fNodalNumDOFs = 0;
    };

    Node(int id, int nodal_num_dofs){
      ID = id;
      fNodalNumDOFs = nodal_num_dofs;
      GenerateDOFs();
    };

    int GetNumDOFs() {
        return fNodalNumDOFs;
    }

    void GenerateDOFs(){
        DOFs = new double[fNodalNumDOFs];
    };

    ~Node() {
        delete[] DOFs;
    };
};

/*
 * Element class for 1D linear and second order elements
 * TODO: Generalize for beam element and 2D plain-strain elements.
 */
class ElementT {

protected:

    int ID;                 /***< Element's ID number */

    double fLength;         /***< Length of the element */
    int fNumNodes;          /***< Number of nodes in the element */
    int fNumDOFs;           /***< Total number of degree of freedoms */

    ArrayT<Node> fNodes;    /***< An array containing element's connectivity nodes */

public:

    /** constructors */
    /* default */
    /*@{*/
    ElementT();
    /*@)*/

    /** Main constructor */
    ElementT(ArrayT<Node>& nodes, int id, const double length);

    /** deconstructor */
    ~ElementT();

};


#endif //SIMPLEBEAM_ELEMENTT_H
