#include"ShellShapes.h"
#include<cmath>
using std::cos;
using std::sin;

static const double CylinderBounds[4] = {0.0, 6.283185307179586476925286766559, -1.0, 1.0};

VertCylinder::VertCylinder(double* cent, double rad, double len) : radius(rad), length(len)
{
    for(int i=0; i<3; ++i)
    {
        center[i] = cent[i];
    }
}

double* VertCylinder::getPosition(double* params)
{
    pos[0] = center[0] + (radius*cos(params[0]));
    pos[1] = center[1] + (radius*sin(params[0]));
    pos[2] = center[2] + params[1]*length;
    return pos;
}

double VertCylinder::getVolumeElement(double* params, double* widths)
{
    return radius*widths[0]*length*widths[1];
}

int VertCylinder::getNumParams()
{
    return 2;
}

const double* VertCylinder::getBounds()
{
    return CylinderBounds;
}

RotXaxisCylinder::RotXaxisCylinder(double* cent, double rad, double len, double angle) : radius(rad), length(len), sEval(sin(angle)), cEval(cos(angle))
{
    for(int i=0; i<3; ++i)
    {
        center[i] = cent[i];
    }
}

double* RotXaxisCylinder::getPosition(double* params)
{
    double x = center[0] + params[1]*length;
    double y = center[1] + (radius*cos(params[0]));
    pos[0] = ((cEval*x) - (sEval*y));
    pos[1] = ((sEval*x) + (cEval*y));
    pos[2] = center[2] + (radius*sin(params[0]));
    return pos;
}

double RotXaxisCylinder::getVolumeElement(double* params, double* widths)
{
    return radius*widths[0]*length*widths[1];
}

int RotXaxisCylinder::getNumParams()
{
    return 2;
}

const double* RotXaxisCylinder::getBounds()
{
    return CylinderBounds;
}
