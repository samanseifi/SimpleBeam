/* Simulating a simple linear beam */
#include <iostream>
#include <vector>
#include <cassert>

extern "C" {
    void dgesv_(int* n, int* nrhs, double* a, int* lda, int* ipiv, double* b, int* ldb, int* info);
}


struct Matrix {
    std::vector<double> data;
    int rows, cols;

    Matrix(int m, int n) : data(m * n), rows(m), cols(n) {}

    double& operator()(int i, int j) {
        return data[i * cols + j];
    }

    void removeRow(int rowToRemove) {
        assert(rowToRemove < rows);
        data.erase(data.begin() + rowToRemove * cols, data.begin() + (rowToRemove + 1) * cols);
        --rows;
    }

    void removeCol(int colToRemove) {
        assert(colToRemove < cols);
        for (int i = rows - 1; i >= 0; --i) {
            data.erase(data.begin() + i * cols + colToRemove);
        }
        --cols;
    }
};

struct Vector {
    std::vector<double> data;

    explicit Vector(int n) : data(n) {}

    double& operator[](int i) {
        return data[i];
    }

    void remove(int indexToRemove) {
        assert(indexToRemove >= 0 && static_cast<size_t>(indexToRemove) < data.size());
        data.erase(data.begin() + indexToRemove);
    }


    int size() const {
        return data.size();
    }
};

void solveLinearSystem(Matrix& A, Vector& b) {
    assert(A.rows == A.cols && b.size() == A.rows); // Ensure system is consistent
    int n = A.rows, nrhs = 1, lda = n, ldb = n, info;
    std::vector<int> ipiv(n);

    dgesv_(&n, &nrhs, &A.data[0], &lda, &ipiv[0], &b.data[0], &ldb, &info);

    if (info > 0) {
        std::cerr << "Solution could not be computed; matrix is singular.\n";
    } else if (info < 0) {
        std::cerr << "Argument " << -info << " had an illegal value.\n";
    }
}




int main() {

    // Beam length 10m
    // Number of elements 10
    // Number of nodes 11
    // Number of DOFs 22
    double L_beam = 10.0;
    int n_elements = 2;
    int n_nodes = n_elements + 1;
    int n_dofs = 3 * n_nodes;

    // create the nodal coordinates
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


    
    double E = 210; // Young's modulus
    double A = 0.01; // Cross-sectional area
    double L = 1.0; // Length of the beam element
    double I = 0.0001; // Moment of inertia

    double C1 = E * A / L;
    double C2 = E * I / L*L*L;

    // Create element stiffness
    Matrix elementStiffness(6, 6);

    elementStiffness(0, 0) = C1;
    elementStiffness(0, 1) = 0.0;
    elementStiffness(0, 2) = 0.0;
    elementStiffness(0, 3) = -C1;
    elementStiffness(0, 4) = 0.0;
    elementStiffness(0, 5) = 0.0;
    
    elementStiffness(1, 0) = 0.0;
    elementStiffness(1, 1) = 12 * C2;
    elementStiffness(1, 2) = 6 * C2 * L;
    elementStiffness(1, 3) = 0.0;
    elementStiffness(1, 4) = -12 * C2;
    elementStiffness(1, 5) = 6 * C2 * L;

    elementStiffness(2, 0) = 0.0;
    elementStiffness(2, 1) = 6 * C2 * L;
    elementStiffness(2, 2) = 4 * C2 * L*L;
    elementStiffness(2, 3) = 0.0;
    elementStiffness(2, 4) = -6 * C2 * L;
    elementStiffness(2, 5) = 2 * C2 * L*L;

    elementStiffness(3, 0) = -C1;
    elementStiffness(3, 1) = 0.0;
    elementStiffness(3, 2) = 0.0;
    elementStiffness(3, 3) = C1;
    elementStiffness(3, 4) = 0.0;
    elementStiffness(3, 5) = 0.0;

    elementStiffness(4, 0) = 0.0;
    elementStiffness(4, 1) = -12 * C2;
    elementStiffness(4, 2) = -6 * C2 * L;
    elementStiffness(4, 3) = 0.0;
    elementStiffness(4, 4) = 12 * C2;
    elementStiffness(4, 5) = -6 * C2 * L;

    elementStiffness(5, 0) = 0.0;
    elementStiffness(5, 1) = 6 * C2 * L;
    elementStiffness(5, 2) = 2 * C2 * L*L;
    elementStiffness(5, 3) = 0.0;
    elementStiffness(5, 4) = -6 * C2 * L;
    elementStiffness(5, 5) = 4 * C2 * L*L;

    // construct the global stiffness matrix
    Matrix globalStiffness(n_dofs, n_dofs);

    for (int i = 0; i < n_elements; i++) {
        int node1 = connectivity[i][0];
        int node2 = connectivity[i][1];

        int dof1 = 3 * node1;
        int dof2 = 3 * node2;

        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 6; k++) {
                if (j < 3 && k < 3) {
                    globalStiffness(dof1 + j % 3, dof1 + k % 3) += elementStiffness(j, k);
                } else if (j < 3) {
                    globalStiffness(dof1 + j % 3, dof2 + k % 3) += elementStiffness(j, k);
                } else if (k < 3) {
                    globalStiffness(dof2 + j % 3, dof1 + k % 3) += elementStiffness(j, k);
                } else {
                    globalStiffness(dof2 + j % 3, dof2 + k % 3) += elementStiffness(j, k);
                }
            }
        }
    }




    // create the force vector
    Vector force(n_dofs);

    // add vertical load at final node
    force[3 * n_nodes - 2] = -1.0;

    // remove corresponding force elements to known displacements
    Vector updatedForces = force;
    updatedForces.remove(0);
    updatedForces.remove(0);
    updatedForces.remove(0);

    

    Matrix updatedStiffness = globalStiffness;
    // Remove cols 0 1 and 2 use removeCol method which is one col at a time
    updatedStiffness.removeCol(0);
    updatedStiffness.removeRow(0);
    updatedStiffness.removeCol(0);
    updatedStiffness.removeRow(0);
    updatedStiffness.removeCol(0);
    updatedStiffness.removeRow(0);



    // globalStiffness.print();
    // std::cout << "A=" << std::endl;
    // updatedStiffness.print();
    // // force.print();
    // std::cout << "b=" << std::endl;
    // updatedForces.print();

    solveLinearSystem(updatedStiffness, updatedForces);

    std::cout << "Solution of the modified system:\n";
    for (int i = 0; i < updatedForces.size(); ++i) {
        std::cout << "x[" << i << "] = " << updatedForces[i] << std::endl;
    }





    return 0;
}


