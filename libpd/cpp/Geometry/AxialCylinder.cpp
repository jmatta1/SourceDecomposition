#include"AxialCylinder.h"
#include<cmath>
using std::cos;
using std::sin;

static const double AxialCylinderBounds[4] = {0.0, 6.283185307179586476925286766559, -1.0, 1.0};

CylinderXaxis::CylinderXaxis(double* cent, double rad, double len) :
    radius(rad), length(len), areaScale(rad*len)
{
    for(int i=0; i<3; ++i)
    {
        center[i] = cent[i];
    }
}

double* CylinderXaxis::getPosition(double* params)
{
    pos[0] = center[0] + params[1]*length;
    pos[1] = center[1] + (radius*cos(params[0]));
    pos[2] = center[2] + (radius*sin(params[0]));
    return pos;
}

double CylinderXaxis::getVolumeElement(double* params, double* widths)
{
    return areaScale*widths[0]*widths[1];
}

int CylinderXaxis::getNumParams()
{
    return 2;
}

const double* CylinderXaxis::getBounds()
{
    return AxialCylinderBounds;
}



CylinderYaxis::CylinderYaxis(double* cent, double rad, double len) :
    radius(rad), length(len), areaScale(rad*len)
{
    for(int i=0; i<3; ++i)
    {
        center[i] = cent[i];
    }
}

double* CylinderYaxis::getPosition(double* params)
{
    pos[0] = center[0] + (radius*cos(params[0]));
    pos[1] = center[1] + params[1]*length;
    pos[2] = center[2] + (radius*sin(params[0]));
    return pos;
}

double CylinderYaxis::getVolumeElement(double* params, double* widths)
{
    return areaScale*widths[0]*widths[1];
}

int CylinderYaxis::getNumParams()
{
    return 2;
}

const double* CylinderYaxis::getBounds()
{
    return AxialCylinderBounds;
}



CylinderZaxis::CylinderZaxis(double* cent, double rad, double len) :
    radius(rad), length(len), areaScale(rad*len)
{
    for(int i=0; i<3; ++i)
    {
        center[i] = cent[i];
    }
}

double* CylinderZaxis::getPosition(double* params)
{
    pos[0] = center[0] + (radius*cos(params[0]));
    pos[1] = center[1] + (radius*sin(params[0]));
    pos[2] = center[2] + params[1]*length;
    return pos;
}

double CylinderZaxis::getVolumeElement(double* params, double* widths)
{
    return areaScale*widths[0]*widths[1];
}

int CylinderZaxis::getNumParams()
{
    return 2;
}

const double* CylinderZaxis::getBounds()
{
    return AxialCylinderBounds;
}
