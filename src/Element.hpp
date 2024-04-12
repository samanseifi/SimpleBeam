#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <iostream>
#include <cmath>
#include "Matrix.hpp"
#include "Node.hpp"
#include "Material.hpp"

class Element {

public:
    Element(Node n1, Node n2, Material material);

    ~Element();

    Matrix getElementStiffness();

private:
    double inclination() const;

    Matrix transformationMatrix() const;

    void formStiffnessMatrix(double E, double A, double L, double I);

private:
    Node node1, node2;
    Material material;    
    Matrix stiffnessMatrix;
    double L;


};

#endif