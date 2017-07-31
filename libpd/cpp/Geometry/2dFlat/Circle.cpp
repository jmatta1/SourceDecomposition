#include"Circle.h"
#include<cmath>
using std::cos;
using std::sin;

static const double CircleBounds[4] = {0.0, 6.283185307179586476925286766559, 0.0, 1.0};

Circle::Circle(double* cent, double r, double* rot) : radius(r), radSquare(r*r)
{
    for(int i=0; i<3; ++i)
    {
        center[i] = cent[i];
        rmat[i] = rot[i];
    }
    for(int i=3; i<9; ++i)
    {
        rmat[i] = rot[i];
    }
}

double* Circle::getPosition(double* params)
{
    double x = radius*cos(params[0]);
    double y = radius*sin(params[0]);
    pos[0] = center[0] + params[1]*(x*rmat[0]+y*rmat[1]);
    pos[1] = center[1] + params[1]*(x*rmat[3]+y*rmat[4]);
    pos[2] = center[2] + params[1]*(x*rmat[6]+y*rmat[7]);
    return pos;
}

double Circle::getVolumeElement(double* params, double* widths)
{
    //The radius range is rmin = radius*(params[1]-widths[1]/2.0) and rmax = radius*(params[1]+widths[1]/2.0)
    //Given that DeltaTheta = widths[0] then you have: that the area = DeltaTheta/2.0*(rmax^2-rmin^2)
    //however this simplifies as follows              (rmax^2-rmin^2) = (rmax-rmin)*(rmax+rmin)
    //since rmax = r+w/2 and rmin=r-w/2 (where r=radius*params[1] and w=radius*widths[1])
    // we then get                             (rmax-rmin)*(rmax+rmin) = (r-r+w/2+w/2)*(r+r+w/2-w/2)
    //this in turn is equal to:                                        = (w)*(2*r) = 2*w*r
    //so then you get area = DeltaThe
    //since for doing the whole circle at once you have params[1] = 0.5 and widths[1] = 1.0 and widths[0] - 2*pi
    //you get the area of the circle ((2*pi)/2)*2*(r/2)*r = pi*r^2
    //therefore the simplified formula becomes: DeltaTheta*r*w
    //again, where DeltaTheta = widths[0], w=widths[1]*radius, r=params[1]*radius
    return std::abs(params[1] * widths[1] * widths[0] * radSquare);
}

int Circle::getNumParams()
{
    return 2;
}

const double* Circle::getBounds()
{
    return CircleBounds;
}
