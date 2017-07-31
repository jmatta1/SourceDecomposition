#include"Square.h"
#include<cmath>
using std::cos;
using std::sin;

static const double SquareBounds[4] = {-1.0, 1.0, -1.0, 1.0};

Square::Square(double* cent, double* v1, double* v2) : mag1(0.0), mag2(0.0)
{
    for(int i=0; i<3; ++i)
    {
        this->center[i] = cent[i];
        this->vec1[i] = v1[i];
        mag1 += (v1[i]*v1[i]);
        this->vec2[i] = v2[i];
        mag2 += (v2[i]*v2[i]);
    }
    mag1 = std::sqrt(mag1);
    mag2 = std::sqrt(mag2);
}

double* Square::getPosition(double* params)
{
    for(int i=0; i<3; ++i)
    {
        pos[i] = center[i]+params[0]*vec1[i]+params[1]*vec2[i];
    }
    return pos;
}

double Square::getVolumeElement(double* params, double* widths)
{
    return std::abs(mag1 * widths[0] * mag2 * widths[1]);
}

int Square::getNumParams()
{
    return 2;
}

const double* Square::getBounds()
{
    return SquareBounds;
}
