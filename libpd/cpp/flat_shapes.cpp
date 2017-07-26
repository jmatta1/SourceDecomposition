#include"flat_shapes.cpp"
#include"cmath"
using std::cos;
using std::sin;


Square::Square(double* cent, double* v1, double* v2)
{
    for(int i=0; i<3; ++i)
    {
        this->center[i] = cent[i];
        this->vec1[i] = v1[i];
        this->vec2[i] = v2[i];
    }
}

double* Square::getPosition(double* params)
{
    for(int i=0; i<3; ++i)
    {
        pos[i] = center[i]+params[0]*vec1[i]+params[1]*vec2[i];
    }
    return pos;
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
    double x = rad*cos(params[0]);
    double y = rad*sin(params[0]);
    pos[0] = center[0] + params[1]*(x*rmat[0]+y*rmat[1]);
    pos[1] = center[1] + params[1]*(x*rmat[3]+y*rmat[4]);
    pos[2] = center[2] + params[1]*(x*rmat[6]+y*rmat[7]);
    return pos;
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
    pos[0] = center[0] + params[1]*(rad*cos(params[0]));
    pos[1] = center[1] + params[1]*(rad*sin(params[0]));
    return pos;
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
    pos[0] = center[0] + params[1]*(rad*cos(params[0]));
    pos[2] = center[2] + params[1]*(rad*sin(params[0]));
    return pos;
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
    pos[1] = center[1] + params[1]*(rad*cos(params[0]));
    pos[2] = center[2] + params[1]*(rad*sin(params[0]));
    return pos;
}
