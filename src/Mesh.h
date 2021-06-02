//
// Created by saman on 6/1/21.
//

#ifndef SIMPLEBEAM_MESH_H
#define SIMPLEBEAM_MESH_H

#include "MatrixT.h"

struct Node {
    int ID;
    double coordX;
};

struct Element {
    ArrayT<Node> vecNodes;
};

class Mesh {
public:
    /** Default Constructor */
    /*@{*/
    Mesh();

    /** Linear element default mesh constructor
     *
     * @param BeamLength : Length of a simple beam
     * @param NumElem : Number of elements to be meshed the beam with
     */
    Mesh(double BeamLength, int NumElem);

    /** Mesh constructor with elmenet order
     *
     * @param BeamLength: Length of a simple beam
     * @param NumElem : Number of elements to be meshed the beam with
     * @param NumNodesPerElem : Number of nodes per element (2= linear interpolation, 3: quadratic interpolation)
     */
    Mesh(double BeamLength, int NumElem, int NumNodesPerElem);
    /*@}*/

    /** Setters and Getters */
    /* Setting up the array of initial nodal coordinates */
    void SetCoords(double BeamLength, double nnodes);

    /* Setting up the connectivity matrix */
    void SetConnectivity(int NumNodesPerElem, int NumElem);

    /* Getting the vector of nodal coordinates */
    VectorT<double> GetCoords();

    /* Getting the connectivity matrix */
    MatrixT<int> GetConnectivity();

    /* Getting the total number of nodes */
    int GetNumNodes();

    /** Default deconstructor */
    ~Mesh();

protected:


    double fBeamLength;         /**< Length of the beam mesh */
    int fNumElem;               /**< Number of elements in the mesh */
    int fNumNodesPerElem;       /**< Number of nodes at each element */

    /* Mesh vectors and matrices to be used in FEM */
    VectorT<double> vecCoords;
    MatrixT<int> matConnect;

};


#endif //SIMPLEBEAM_MESH_H
