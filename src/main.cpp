/* Simulating a simple linear beam */

#include "Mesh.h"

using namespace std;

/* Assigning the weighting for numerical integration */
void GetWeights(vector<double> &xi, vector<double> &w, int numIP);



int main() {

    /** \name Geometry: Area simple bat with a given beam_length and cross section area = Area  */
    const double beam_length = 5.0;
    const double Area = 1.0;

    /** \name Material properties
     *          mu: Shear modulus
     *          nu: Poisson Ratio
     **/
    /*@{*/
    const double mu = 50.0;
    const double nu = 0.3;
    const double cnst = 2.0 * mu * Area * (1 - nu) / (1 - 2 * nu);
    /*@}*/

    /** \name Defining Loadings */
    /*@{*/
    const double bodyforce = 10.0;
    const double traction = 2.0;
    /*@}*/

    /** Total # of elements (nel) and # of nodes per element (2 for linear, 3 for quadratic elements) */
    /*@{*/
    int nel = 10;
    int nnodes_per_el = 3;
    /*@}*/

    Mesh mesh_beam(beam_length, nel, nnodes_per_el);

    /** Setting up the data structures for the mesh */
    /*@{*/
    /* 1) Building Vector of nodal coordinates */
    VectorT<double> vecCoords = mesh_beam.GetCoords();


    /* 2) Building matrix of element connectivity (specifies node numbers on each element) */
    MatrixT<int> matConnect = mesh_beam.GetConnectivity();
    /*@{*/

    int nnodes = mesh_beam.GetNumNodes();

    /* Integration points and weights for 2 point integration */
    /*@{*/
    int numIP = nnodes_per_el - 1;
    vector<double> xi(2), w(2);
    GetWeights(xi, w, numIP);
    /*@{*/

    /* Assemble the global stiffness and force vector */
    MatrixT<double> K(nnodes);
    VectorT<double> F(nnodes);

    /* Loop over elements! */
    for (int elem = 0; elem < nel; elem++) {

        /* Extract the vecCoords of each node on the current element */
        MatrixT<double> K_el;
        K_el.Dimension(nnodes_per_el, nnodes_per_el);

        VectorT<double> F_el;
        F_el.Dimension(nnodes_per_el);

        /* Extract nodal coordinates of each element */
        VectorT<double> vecElemCoords;
        vecElemCoords.Dimension(nnodes_per_el);

        for (int a = 0; a < nnodes_per_el; a++) {
            vecElemCoords[a] = vecCoords[matConnect(a, elem)];
        }

        /* Loop over number of integration points for the current element and assemble element stiffness!  */
        for (int II = 0; II < numIP; II++) {

            /* Compute shape functions and its derivatives: N and dN/dxi at the current integration point */
            VectorT<double> N;
            N.Dimension(nnodes_per_el);

            VectorT<double> dNdxi;
            dNdxi.Dimension(nnodes_per_el);

            /* Element order = number of nodes - 1 */
            if (nnodes_per_el == 3) {
                N[0] = -0.5 * xi[II] * (1.0 - xi[II]);
                N[1] =  0.5 * xi[II] * (1.0 + xi[II]);
                N[2] =  1.0 - pow(xi[II], 2);
                dNdxi[0] = -0.5 + xi[II];
                dNdxi[1] =  0.5 + xi[II];
                dNdxi[2] = -2.0 * xi[II];
            } else if (nnodes_per_el == 2) {
                N[0] = 0.5 * (1.0 - xi[II]);
                N[1] = 0.5 * (1.0 + xi[II]);
                dNdxi[0] = -0.5;
                dNdxi[1] =  0.5;
            }

            /* Compute dx/dxi, J and dN/dx */
            double dxdxi = 0.0;
            for (int a = 0; a < nnodes_per_el; a++) {
                dxdxi = dxdxi + dNdxi[a] * vecElemCoords[a];
            }
            /* Calculate the Jacobian */
            double J = abs(dxdxi);

            VectorT<double> dNdx;
            dNdx.Dimension(nnodes_per_el);
            for (int a = 0; a < nnodes_per_el; a++) {
                dNdx[a] = dNdxi[a] / dxdxi;
            }

            /* Add contribution to element stiffness and force vector from current integration pt */
            for (int a = 0; a < nnodes_per_el; a++) {
                F_el[a] += w[II] * bodyforce * J * N[a];
                for (int b = 0; b < nnodes_per_el; b++) {
                    K_el(a, b) += cnst * w[II] * J * dNdx[a] * dNdx[b];
                }
            }
        }

        /* Add the stiffness and residual from the current element into global matrices */
        for (int a = 0; a < nnodes_per_el; a++) {
            int rw = matConnect(a, elem);
            F[rw] += F_el[a];
            for (int b = 0; b < nnodes_per_el; b++) {
                int cl = matConnect(b, elem);
                K(rw, cl) += K_el(a, b);
            }
        }
    }

    /*  Add the extra forcing term from the traction at x=nel */
    //TODO: It should be generalized to a set of nodes.
    F[nnodes-1] = F[nnodes-1] + traction;

    /**
     * Modify FEM equations to enforce displacement boundary condition
     * To do this we simply replace the equation for the first node with u=0
     **/
     //TODO: This also should be generalized to a set of nodes.
    for (int a = 0; a < nnodes; a++)
        K(0, a) = 0.0;

    // K(0,0) = 1 means when it is multiplied by the
    K(0, 0) = 1.0;
    F[0] = 0.0;
    K.Print();



    /* Solving the linear system */


    //SolveLinear(sol, K, F);

    return 0;
}

/* Getting the weights for numerical integration */
void GetWeights(vector<double> &xi, vector<double> &w, const int numIP) {
    if (numIP == 2) {
        w[0] = w[1] = 1.0;
        xi[0] = -0.5773502692;
        xi[1] =  0.5773502692;
    } else if (numIP == 1) {
        w[0] = 2.0;
        w[1] = 0.0;
        xi[0] = xi[1] = 0.0;
    }
}


