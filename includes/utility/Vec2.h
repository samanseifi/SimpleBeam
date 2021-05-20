//
// Created by saman on 10/29/20.
//

#ifndef SIMPLEWIRE_DVEC2D_H
#define SIMPLEWIRE_DVEC2D_H

#include <iostream>
#include <vector>

#include <cmath>

using namespace std;

/* Simple double precision 2D cartesian vector object for simple mass/spring calculations */
class Vec2 {
public:
    Vec2() {
        this->x = 0.0;
        this->y = 0.0;
    };

    Vec2(double x1, double y1) {
        this->x = x1;
        this->y = y1;
    };

    // Because implicitly-declaration is deprecated
    Vec2(const Vec2 &obj) {
        this->x = obj.x;
        this->y = obj.y;
    };

    inline Vec2& operator=(Vec2 const& v) {
        if (this != &v){
            x = v.x;
            y = v.y;
        }
        return *this;
    };

    inline Vec2& operator=(double const &val) {
        this->x = val;
        this->y = val;

        return *this;
    };

    inline Vec2 operator+(const Vec2& v) const {
        return Vec2(x + v.x, y + v.y);
    };

    inline Vec2& operator*(const double& val) {
        this->x = val*x;
        this->y = val*y;
        return *this;
    };

    inline Vec2 setToScale(const double& scale) const {
        return Vec2(x * scale, y * scale);

    };

    inline Vec2& operator+=(Vec2 &v) {
        this->x = x + v.x;
        this->y = y + v.y;
        return *this;
    };

    inline Vec2& operator*=(double const& scale) {
        this->x = x*scale;
        this->y = y*scale;
        return *this;
    };

    inline Vec2 operator-(const Vec2& v) const {
        return Vec2(x - v.x, y - v.y);
    };

    double Magnitude() const {
        return sqrt(x*x + y*y);
    };

    Vec2 UnitVec() const {
      return Vec2(x / Magnitude(), y / Magnitude());
    };

    void PrintVec() const{
        std::cout <<"("<< x << ", " << y << ")" << std::endl;
    };

    double Dot(const Vec2& v) const {
        return x*v.x + y*v.y;
    };

    ~Vec2() = default;

    double y;
    double x;
};




#endif //SIMPLEWIRE_DVEC2D_H
