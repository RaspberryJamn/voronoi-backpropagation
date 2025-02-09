#ifndef RGBCOLOR_H
#define RGBCOLOR_H
#include <cmath>
#include "IdkFunctions.h"

struct RGBColor {
    double r;
    double g;
    double b;
    RGBColor() : r(0), g(0), b(0) {}
    RGBColor(double r, double g, double b) : r(r), g(g), b(b) {}
    RGBColor operator+(RGBColor const& rhs) const {
        return RGBColor(r+rhs.r,g+rhs.g,b+rhs.b);
    }
    void operator+=(RGBColor const& rhs) {
        r += rhs.r; g += rhs.g; b += rhs.b;
    }
    RGBColor operator-(RGBColor const& rhs) const {
        return RGBColor(r-rhs.r,g-rhs.g,b-rhs.b);//RGBColor result = RGBColor(r-rhs.r,g-rhs.g,b-rhs.b); return result; //
    }
    void operator-=(RGBColor const& rhs) {
        r -= rhs.r; g -= rhs.g; b -= rhs.b;
    }
    RGBColor operator*(RGBColor const& rhs) const {
        return RGBColor(r*rhs.r,g*rhs.g,b*rhs.b);//RGBColor result = RGBColor(r*rhs.r,g*rhs.g,b*rhs.b); return result; //
    }
    RGBColor operator/(RGBColor const& rhs) const {
        RGBColor result = RGBColor(this->r*((std::abs(rhs.r)<0.0001) ? ((rhs.r>0)?1:-1)*10000.0 : 1.0/rhs.r),
                                   this->g*((std::abs(rhs.g)<0.0001) ? ((rhs.g>0)?1:-1)*10000.0 : 1.0/rhs.g),
                                   this->b*((std::abs(rhs.b)<0.0001) ? ((rhs.b>0)?1:-1)*10000.0 : 1.0/rhs.b));
        return result; //return RGBColor(r/rhs.r,g/rhs.g,b/rhs.b);
    }
    RGBColor operator*(double const& rhs) const {
        return RGBColor(r*rhs,g*rhs,b*rhs);//RGBColor result = RGBColor(r*rhs,g*rhs,b*rhs); return result; //
    }
    static double Trace(RGBColor const& col) { // more taxicab magnitude but whatever
        return col.r+col.g+col.b;
    }
    void Clamp() {
        G_Clamp<double>(&this->r, 0.01, 255);
        G_Clamp<double>(&this->g, 0.01, 255);
        G_Clamp<double>(&this->b, 0.01, 255);
    }
};

#endif // RGBCOLOR_H
