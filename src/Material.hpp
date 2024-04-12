struct Material {
    double E, A, I;

    // default constructor
    Material() : E(0.0), A(0.0), I(0.0) {}

    // constructor
    Material(double E, double A, double I) : E(E), A(A), I(I) {}

};