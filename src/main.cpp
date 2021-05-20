/* Simulating a simple linear beam */

#include "MatrixT.h"

using namespace std;

/* Calculating total number of nodes with num. of nodes at each element Ne and number of elements L */
int GetNumNodes(int Ne, int L) {
    return (Ne - 1)*L + 1;
}

/* Assigning the weighting for numerical integration */
void GetWeights(vector<double> &xi, vector<double> &w, int numIP);

/* Building the array of initial nodal coordinates */
void get_coords(ArrayT<double> &coords, double Length, double nnodes);

/* Building the connectivity matrix */
void get_connectivity(MatrixT<int> &connect, int Ne, int L);


int main()  {

    /** \name Geometry: A simple bat with a given Length and cross section area = A  */
    const double Length = 5.0;
    const double A = 1.0;

    /** \name Material properties
     *          mu: Shear modulus
     *          nu: Poisson Ratio
     **/
    /*@{*/
    const double mu = 50.0;
    const double nu = 0.3;
    const double cnst = 2.0*mu*A*(1-nu)/(1-2*nu);
    /*@{*/

    /** Defining Loadings */
    /*@{*/
    const double bodyforce = 10.0;
    const double traction = 2.0;
    /*@{*/

    /** total no. elements, no. nodes on each element (2 for linear, 3 for quadratic elements) */
    /*@{*/
    int L = 10;
    int Ne = 3;
    /*@{*/

    /** Get total number of nodes */
    int nnodes = GetNumNodes(Ne, L);

    /** Setting up the data structures for the mesh */
    /*@{*/
    /* 1) Building Vector of nodal coordinates */
    VectorT<double> coords(nnodes);
    get_coords(coords, Length, nnodes);

    /* 2) Building matrix of element connectivity (specifies node numbers on each element) */
    MatrixT<int> connect(Ne, L);
    get_connectivity(connect, Ne, L);
    /*@{*/

    /* Integration points and weights for 2 point integration */
    /*@{*/
    int numIP = Ne - 1;
    vector<double> xi(2), w(2);
    GetWeights(xi, w, numIP);
    /*@{*/

    /* Assemble the global stiffness and force vector */
    MatrixT<double> K(nnodes);
    ArrayT<double> F(nnodes);

    for (int lmn = 0; lmn < L; lmn++) {
        //       Extract the coords of each node on the current element
        MatrixT<double> kel(Ne, Ne);
        ArrayT<double> fel(Ne);
        ArrayT<double> lmncoords(Ne);
        for (int a = 0; a < Ne; a++) {
            lmncoords[a] = coords[connect(a, lmn)];
        }
        //      For the current element, loop over integration points and assemble element stiffness

        for (int II = 0; II < numIP; II++) {
            //        Compute N and dN/dxi at the current integration point
            ArrayT<double> N(Ne);
            ArrayT<double> dNdxi(Ne);
            if (Ne == 3) {
                N[0] = -0.5 * xi[II] * (1.0 - xi[II]);
                N[1] = 0.5 * xi[II] * (1.0 + xi[II]);
                N[2] = (1. - pow(xi[II], 2));
                dNdxi[0] = -0.5 + xi[II];
                dNdxi[1] = 0.5 + xi[II];
                dNdxi[2] = -2. * xi[II];
            } else if (Ne == 2) {
                N[0] = 0.5 * (1. - xi[II]);
                N[1] = 0.5 * (1. + xi[II]);
                dNdxi[0] = -0.5;
                dNdxi[1] = 0.5;
            }
            //        Compute dx/dxi, J and dN/dx
            double dxdxi = 0.0;
            for (int a = 0; a < Ne; a++) {
                dxdxi = dxdxi + dNdxi[a] * lmncoords[a];
            }
            double J = abs(dxdxi);
            ArrayT<double> dNdx(Ne);
            for (int a = 0; a < Ne; a++) {
                dNdx[a] = dNdxi[a] / dxdxi;
            }
            //         Add contribution to element stiffness and force vector from current integration pt
            for (int a = 0; a < Ne; a++) {
                fel[a] = fel[a] + w[II] * bodyforce * J * N[a];
                for (int b = 0; b < Ne; b++) {
                    kel(a, b) = kel(a, b) + cnst * w[II] * J * dNdx[a] * dNdx[b];
                }
            }
        }

        //         Add the stiffness and residual from the current element into global matrices
        for (int a = 0; a < Ne; a++) {
            int rw = connect(a, lmn);
            F[rw] = F[rw] + fel[a];
            for (int b = 0; b < Ne; b++) {
                int cl = connect(b, lmn);
                K(rw, cl) = K(rw, cl) + kel(a, b);
            }
        }
    }


    //     Add the extra forcing term from the traction at x=L
    F[nnodes-1] = F[nnodes-1] + traction;

    //      Modify FEM equations to enforce displacement boundary condition
    //      To do this we simply replace the equation for the first node with u=0

    for (int a = 0; a < nnodes; a++)
        K(0,a) = 0.0;

    K(0,0) = 1.;
    F[0] = 0.;

    K.Print();


    return 0;
}

void GetWeights(vector<double> &xi, vector<double> &w, const int numIP) {
    if (numIP == 2) {
        w[0] = w[1] = 1.0;
        xi[0] = -0.5773502692;
        xi[1] = 0.5773502692;
    } else if (numIP == 1) {
        w[0] = 2.0;
        w[1] = 0.0;
        xi[0] = xi[1] = 0.0;
    }
}

void get_coords(ArrayT<double> &coords, const double Length, const double nnodes) {
    for (int i = 0; i < nnodes; i++)
        coords[i] = Length*(i)/(nnodes-1);
}

void get_connectivity(MatrixT<int> &connect, const int Ne, const int L) {
    for (int lmn = 0; lmn < L; lmn++) {
        if (Ne == 3){
            /* 2n order element with 3 nodes: O----O----O */
            connect(0, lmn) = 2*lmn;
            connect(1, lmn) = 2*lmn+1;
            connect(2, lmn) = 2*lmn+2;
        } else if (Ne == 2) {
            /* 1st order element with 2 nodes: O-----O */
            connect(0, lmn) = lmn;
            connect(1, lmn) = lmn+1;
        }
    }
}
