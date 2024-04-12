/* Simulating a simple linear beam */
#include <iostream>
#include <cmath>
#include <algorithm>
#include "Vector.hpp"
#include "Element.hpp"

extern "C" {
    void dgesv_(int* n, int* nrhs, double* a, int* lda, int* ipiv, double* b, int* ldb, int* info);
}

Vector solveLinearSystem(Matrix& A, const Vector& b) {
    if (A.rows != A.cols || b.size() != A.rows) {
        throw std::runtime_error("Inconsistent system: Matrix dimensions and vector size do not match.");
    }
    int n = A.rows, nrhs = 1, lda = n, ldb = n, info;
    std::vector<int> ipiv(n);
    Vector x(n);

    std::copy(b.data.begin(), b.data.end(), x.data.begin());

    dgesv_(&n, &nrhs, &A.data[0], &lda, &ipiv[0], &x.data[0], &ldb, &info);

    if (info > 0) {
        std::cerr << "Solution could not be computed; matrix is singular.\n";
    } else if (info < 0) {
        std::cerr << "Argument " << -info << " had an illegal value.\n";
    }

    return x;
}

void globalStiffnessMatrix(Matrix& globalStiffness, std::vector<Element> elements, int connectivity[][2]) {
    for (int i = 0; i < elements.size(); i++) {
        
        auto stiffnessMatrix = elements[i].getElementStiffness();
        
        int node1 = connectivity[i][0];
        int node2 = connectivity[i][1];

        int dof1 = 3 * node1;
        int dof2 = 3 * node2;

        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 6; k++) {
                if (j < 3 && k < 3) {
                    globalStiffness(dof1 + j % 3, dof1 + k % 3) += stiffnessMatrix(j, k);
                } else if (j < 3) {
                    globalStiffness(dof1 + j % 3, dof2 + k % 3) += stiffnessMatrix(j, k);
                } else if (k < 3) {
                    globalStiffness(dof2 + j % 3, dof1 + k % 3) += stiffnessMatrix(j, k);
                } else {
                    globalStiffness(dof2 + j % 3, dof2 + k % 3) += stiffnessMatrix(j, k);
                }
            }
        }
    }
}


int main() {

    // Beam length 10m
    // Number of elements 10
    // Number of nodes 11
    // Number of DOFs 22
    double L_beam = 10.0;
    int n_elements = 10;
    int n_nodes = n_elements + 1;
    int n_dofs = 3 * n_nodes;


    double E = 210e9; // Young's modulus unit Pa
    double A = 0.01; // Cross-sectional area unit m^2
    double L = L_beam / n_elements; // Element length unit m
    double I = 0.0001; // Moment of inertia unit m^4

    Material material(E, A, I);

    // create Element objects
    std::vector<Element> elements;
    for (int i = 0; i < n_elements; i++) {
        Node n1, n2;
        n1.x = i * L_beam / n_elements;
        n1.y = 0.0;
        n2.x = (i + 1) * L_beam / n_elements;
        n2.y = 0.0;
        elements.emplace_back(n1, n2, material);
    }
    // get coordinates
    Matrix coordinates(n_nodes, 2);
    for (int i = 0; i < n_nodes; i++) {
        coordinates(i, 0) = i * L_beam / n_elements;
        coordinates(i, 1) = 0.0;
    }

    // Mesh connectivity
    int connectivity[n_elements][2];
    for (int i = 0; i < n_elements; i++) {
        connectivity[i][0] = i;
        connectivity[i][1] = i + 1;
    }

    // construct the global stiffness matrix
    Matrix globalStiffness(n_dofs, n_dofs);
    globalStiffnessMatrix(globalStiffness, elements, connectivity);

    // create the force vector
    Vector force(n_dofs);

    // add vertical load at final node
    force[3 * n_nodes - 2] = -1.0;

    // remove corresponding force elements to known displacements
    Vector updatedForces = force;
    updatedForces.remove({0, 1, 2});

    Matrix updatedStiffness = globalStiffness;
    updatedStiffness.removeCols({0, 1, 2});
    updatedStiffness.removeRows({0, 1, 2});


    auto updatedDisplacements = solveLinearSystem(updatedStiffness, updatedForces);

    std::cout << "Solution of the modified system:\n";
    for (int i = 0; i < updatedDisplacements.size(); ++i) {
        std::cout << "x[" << i << "] = " << updatedDisplacements[i] << std::endl;
    }

    // plot deforrmed shape for both x and y dofs
    for (int i = 0; i < n_nodes; i++) {
        double x = coordinates(i, 0);
        double y = coordinates(i, 1);
        double u = 0.0;
        double v = 0.0;
        if (i > 0) {
            u = updatedDisplacements[3 * i - 3];
            v = updatedDisplacements[3 * i - 2];
        }
        std::cout << x + u << " " << y + v << std::endl; // unit m
    }
    
    

    return 0;
}


