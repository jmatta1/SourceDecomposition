#include"PlanarCircles.h"
#include<cmath>
using std::cos;
using std::sin;

static const double PlanarCircleBounds[4] = {0.0, 6.283185307179586476925286766559, 0.0, 1.0};

CircleXY::CircleXY(double* cent, double r) : radius(r), radSquare(r*r)
{
    for(int i=0; i<3; ++i)
    {
        center[i] = cent[i];
    }
    //since the r-vector never contributes to the z axis, we can precalculate
    //the z component of the position vector and ignore it thereafter
    pos[2] = center[2];
}

double* CircleXY::getPosition(double* params)
{
    pos[0] = center[0] + params[1]*(radius*cos(params[0]));
    pos[1] = center[1] + params[1]*(radius*sin(params[0]));
    return pos;
}

double CircleXY::getVolumeElement(double* params, double* widths)
{
    // see Circle::getVolumeElement for explanation
    return std::abs(params[1]*widths[1]*widths[0]*radSquare);
}

int CircleXY::getNumParams()
{
    return 2;
}

const double* CircleXY::getBounds()
{
    return PlanarCircleBounds;
}



CircleXZ::CircleXZ(double* cent, double r) : radius(r), radSquare(r*r)
{
    for(int i=0; i<3; ++i)
    {
        center[i] = cent[i];
    }
    //since the r-vector never contributes to the y axis, we can precalculate
    //the y component of the position vector and ignore it thereafter
    pos[1] = center[1];
}

double* CircleXZ::getPosition(double* params)
{
    pos[0] = center[0] + params[1]*(radius*cos(params[0]));
    pos[2] = center[2] + params[1]*(radius*sin(params[0]));
    return pos;
}

double CircleXZ::getVolumeElement(double* params, double* widths)
{
    // see Circle::getVolumeElement for explanation
    return std::abs(params[1]*widths[1]*widths[0]*radSquare);
}

int CircleXZ::getNumParams()
{
    return 2;
}

const double* CircleXZ::getBounds()
{
    return PlanarCircleBounds;
}



CircleYZ::CircleYZ(double* cent, double r) : radius(r), radSquare(r*r)
{
    for(int i=0; i<3; ++i)
    {
        center[i] = cent[i];
    }
    //since the r-vector never contributes to the x axis, we can precalculate
    //the x component of the position vector and ignore it thereafter
    pos[0] = center[0];
}

double* CircleYZ::getPosition(double* params)
{
    pos[1] = center[1] + params[1]*(radius*cos(params[0]));
    pos[2] = center[2] + params[1]*(radius*sin(params[0]));
    return pos;
}

double CircleYZ::getVolumeElement(double* params, double* widths)
{
    // see Circle::getVolumeElement for explanation
    return std::abs(params[1]*widths[1]*widths[0]*radSquare);
}

int CircleYZ::getNumParams()
{
    return 2;
}

const double* CircleYZ::getBounds()
{
    return PlanarCircleBounds;
}

