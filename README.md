# SimpleBeam

SimpleBeam is a lightweight, efficient Finite Element Method (FEM) solver specifically designed for analyzing Euler-Bernoulli beam problems. This application provides a streamlined approach for engineers and researchers to perform structural analysis on beam elements subject to various loading conditions.

## Key Features
- **Euler-Bernoulli Beam Solver:** Capable of solving beam problems, including calculation of deflections, bending moments, shear forces, and stresses.
- **LAPACKE Integration:** Utilizes LAPACKE, a C interface to LAPACK, for efficient solving of large linear systems, ensuring high performance and accuracy.
- **Custom Linear Algebra Tools:** Implements its own Vector and Matrix classes built on top of `std::vector` to facilitate essential operations such as multiplication, transposition, and dynamic modification (adding/removing columns and rows). These tools are tailored to meet the specific needs of finite element system construction.

## Example
He is an example of SimpleBeam soving for a beam with one end fixed and the other end a vertical downward force $F=-0.1\ \text{N}$. The beam has the following geometrical and material properties $L=10\ \text{m}$, $A=0.01\ \text{m}^2$, $E=210\times 10^9\ \text{Pa}$, $I=0.0001\ \text{m}^4$. 


![image](https://github.com/samanseifi/SimpleBeam/assets/9206261/9be83fc8-6e36-4c27-8e9c-b4d24a12ccc2)

