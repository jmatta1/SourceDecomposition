#include"flat_shapes.h"
#include<cmath>
using std::cos;
using std::sin;
#include<iostream>
using std::cout;
using std::endl;

static const double SquareBounds[4] = {-1.0, 1.0, -1.0, 1.0};
static const double CircleBounds[4] = {0.0, 6.283185307179586476925286766559, 0.0, 1.0};

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




Circle::Circle(double* cent, double r, double* rot) : radius(r)
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
    return std::abs(params[1]*widths[1]*widths[0]*radius*radius);
}

int Circle::getNumParams()
{
    return 2;
}

const double* Circle::getBounds()
{
    return CircleBounds;
}





CircleXY::CircleXY(double* cent, double r) : radius(r)
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
    return std::abs(params[1]*widths[1]*widths[0]*radius*radius);
}

int CircleXY::getNumParams()
{
    return 2;
}

const double* CircleXY::getBounds()
{
    return CircleBounds;
}






CircleXZ::CircleXZ(double* cent, double r) : radius(r)
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
    return std::abs(params[1]*widths[1]*widths[0]*radius*radius);
}

int CircleXZ::getNumParams()
{
    return 2;
}

const double* CircleXZ::getBounds()
{
    return CircleBounds;
}





CircleYZ::CircleYZ(double* cent, double r) : radius(r)
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
    return std::abs(params[1]*widths[1]*widths[0]*radius*radius);
}

int CircleYZ::getNumParams()
{
    return 2;
}

const double* CircleYZ::getBounds()
{
    return CircleBounds;
}

