#include "Element.hpp"

Element::Element(Node n1, Node n2, Material material) : node1(n1), node2(n2) {
    L = sqrt(pow(node2.x - node1.x, 2) + pow(node2.y - node1.y, 2));
    
    formStiffnessMatrix(material.E, material.A, L, material.I);
}

Element::~Element() {}

Matrix Element::getElementStiffness() {
    return stiffnessMatrix;
}

double Element::inclination() const {
    return atan2(node2.y - node1.y, node2.x - node1.x);
}

Matrix Element::transformationMatrix() const {
    double c = cos(inclination());
    double s = sin(inclination());

    Matrix T(6, 6);
    T(0, 0) = c; T(0, 1) = s; T(0, 2) = 0.0; T(0, 3) = 0.0; T(0, 4) = 0.0; T(0, 5) = 0.0;
    T(1, 0) = -s; T(1, 1) = c; T(1, 2) = 0.0; T(1, 3) = 0.0; T(1, 4) = 0.0; T(1, 5) = 0.0;
    T(2, 0) = 0.0; T(2, 1) = 0.0; T(2, 2) = 1.0; T(2, 3) = 0.0; T(2, 4) = 0.0; T(2, 5) = 0.0;
    T(3, 0) = 0.0; T(3, 1) = 0.0; T(3, 2) = 0.0; T(3, 3) = c; T(3, 4) = s; T(3, 5) = 0.0;
    T(4, 0) = 0.0; T(4, 1) = 0.0; T(4, 2) = 0.0; T(4, 3) = -s; T(4, 4) = c; T(4, 5) = 0.0;
    T(5, 0) = 0.0; T(5, 1) = 0.0; T(5, 2) = 0.0; T(5, 3) = 0.0; T(5, 4) = 0.0; T(5, 5) = 1.0;

    return T;
}

void Element::formStiffnessMatrix(double E, double A, double L, double I) {
    double C1 = E * A / L;
    double C2 = E * I / pow(L, 3);

    Matrix stiffnessMatrix(6, 6);

    stiffnessMatrix(0, 0) =  C1; stiffnessMatrix(0, 1) = stiffnessMatrix(0, 2) = 0.0;
    stiffnessMatrix(0, 3) = -C1; stiffnessMatrix(0, 4) = stiffnessMatrix(0, 5) = 0.0;
    
    stiffnessMatrix(1, 0) = 0.0; stiffnessMatrix(1, 1) =  12 * C2; stiffnessMatrix(1, 2) = 6 * C2 * L;
    stiffnessMatrix(1, 3) = 0.0; stiffnessMatrix(1, 4) = -12 * C2; stiffnessMatrix(1, 5) = 6 * C2 * L;

    stiffnessMatrix(2, 0) = 0.0; stiffnessMatrix(2, 1) =  6 * C2 * L; stiffnessMatrix(2, 2) = 4 * C2 * L*L; 
    stiffnessMatrix(2, 3) = 0.0; stiffnessMatrix(2, 4) = -6 * C2 * L; stiffnessMatrix(2, 5) = 2 * C2 * L*L;

    stiffnessMatrix(3, 0) = -C1; stiffnessMatrix(3, 1) = stiffnessMatrix(3, 2) = 0.0;
    stiffnessMatrix(3, 3) =  C1; stiffnessMatrix(3, 4) = stiffnessMatrix(3, 5) = 0.0;

    stiffnessMatrix(4, 0) = 0.0; stiffnessMatrix(4, 1) = -12 * C2; stiffnessMatrix(4, 2) = -6 * C2 * L;
    stiffnessMatrix(4, 3) = 0.0; stiffnessMatrix(4, 4) =  12 * C2; stiffnessMatrix(4, 5) = -6 * C2 * L;

    stiffnessMatrix(5, 0) = 0.0; stiffnessMatrix(5, 1) =  6 * C2 * L; stiffnessMatrix(5, 2) = 2 * C2 * L*L;
    stiffnessMatrix(5, 3) = 0.0; stiffnessMatrix(5, 4) = -6 * C2 * L; stiffnessMatrix(5, 5) = 4 * C2 * L*L;

    // if the element is inclined
    if (inclination() != 0.0) {
        Matrix T = transformationMatrix();
        
        Matrix T_transspose = T;
        T_transspose.transpose();

        stiffnessMatrix.MultAB(T_transspose, stiffnessMatrix);
        stiffnessMatrix.MultAB(stiffnessMatrix, T);
    }

    this->stiffnessMatrix = stiffnessMatrix;
}
