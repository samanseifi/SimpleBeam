//
// Created by saman on 5/17/21.
//

#include "ElementT.h"

// Default constructor
ElementT::ElementT():
    ID(0),
    fLength(0),
    fNumNodes(0),
    fNumDOFs(0)
{

}

ElementT::~ElementT() {

}

// Main constructor
ElementT::ElementT(ArrayT <Node> &nodes, int id, const double length) {

    ID = id;
    fLength = length;
    fNodes = nodes;

    /* Finding the total number of DOFs */
    for (int i = 0; i < nodes.Length(); i++) {
        fNumDOFs += i*nodes[i].GetNumDOFs();
    }
}
