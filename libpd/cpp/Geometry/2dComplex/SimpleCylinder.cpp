#include"SimpleCylinder.h"
#include<cmath>
using std::cos;
using std::sin;

static const double SimpleCylinderBounds[4] = {0.0, 6.283185307179586476925286766559, -1.0, 1.0};

CylinderXaxisRotatedZ::CylinderXaxisRotatedZ(double* cent, double rad, double len, double angle) :
    radius(rad), length(len), areaScale(rad*len), sEval(sin(angle)), cEval(cos(angle))
{
    for(int i=0; i<3; ++i)
    {
        center[i] = cent[i];
    }
}

double* CylinderXaxisRotatedZ::getPosition(double* params)
{
    double x = center[0] + params[1]*length;
    double y = center[1] + (radius*cos(params[0]));
    pos[0] = ((cEval*x) - (sEval*y));
    pos[1] = ((sEval*x) + (cEval*y));
    pos[2] = center[2] + (radius*sin(params[0]));
    return pos;
}

double CylinderXaxisRotatedZ::getVolumeElement(double* params, double* widths)
{
    return areaScale*widths[0]*widths[1];
}

int CylinderXaxisRotatedZ::getNumParams()
{
    return 2;
}

const double* CylinderXaxisRotatedZ::getBounds()
{
    return SimpleCylinderBounds;
}
