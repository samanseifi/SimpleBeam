//
// Created by saman on 6/1/21.
//

#include "Mesh.h"

Mesh::Mesh():
    fBeamLength(0),
    fNumElem(0),
    fNumNodesPerElem(0),
    vecCoords(),
    matConnect()
{

}

Mesh::Mesh(double BeamLength, int NumElem) {
    fBeamLength = BeamLength;
    fNumElem = NumElem;
    fNumNodesPerElem = 2;

    int nnodes = this->GetNumNodes();
    SetCoords(BeamLength, nnodes);
    SetConnectivity(fNumNodesPerElem, fNumElem);
}

Mesh::Mesh(double BeamLength, int NumElem, int NumNodesPerElem) {
    fBeamLength = BeamLength;
    fNumElem = NumElem;
    fNumNodesPerElem = NumNodesPerElem;

    int nnodes = this->GetNumNodes();
    SetCoords(BeamLength, nnodes);
    SetConnectivity(fNumNodesPerElem, fNumElem);

}

/* Getting the number of nodes from number of elements and number of nodes per element */
int Mesh::GetNumNodes() {
    return (fNumNodesPerElem - 1)*fNumElem + 1;
}

/* Creating the vector of nodal coordinates */
void Mesh::SetCoords(const double BeamLength, const double nnodes) {

    vecCoords.Dimension(nnodes);

    for (int i = 0; i < nnodes; i++)
        vecCoords[i] = BeamLength * (i) / (nnodes - 1);
}

/* Creating connectivity matrix! */
void Mesh::SetConnectivity(const int NumNodesPerElem, const int NumElem) {

    matConnect.Dimension(fNumNodesPerElem, fNumElem);

    for (int lmn = 0; lmn < NumElem; lmn++) {
        if (NumNodesPerElem == 3){

            /* 2n order element with 3 nodes: O----O----O */
            matConnect(0, lmn) = 2*lmn;
            matConnect(1, lmn) = 2*lmn+1;
            matConnect(2, lmn) = 2*lmn+2;
        } else if (NumNodesPerElem == 2) {

            /* 1st order element with 2 nodes: O-----O */
            matConnect(0, lmn) = lmn;
            matConnect(1, lmn) = lmn+1;
        }
    }
}

/* Returning the vector of coordinates */
VectorT<double> Mesh::GetCoords() {
    return vecCoords;
}

/* Returning the matrix of connectivity */
MatrixT<int> Mesh::GetConnectivity() {
    return matConnect;
}
