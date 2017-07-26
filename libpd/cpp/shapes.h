#ifndef POSITION_DECOMP_LIBPD_CPP_SHAPES_H
#define POSITION_DECOMP_LIBPD_CPP_SHAPES_H

// the abstract base class for the various shape classes
class Shape
{
public:
    virtual ~Shape(){}
    virtual double* getPosition(double*) = 0;
};

class Square : public Shape
{
public:
    Square(double* center, double* vec1, double* vec2);
    virtual ~Square(){}
    virtual double* getPosition(double*);

private:
    double center[3];
    double vec1[3];
    double vec2[3];
    double pos[3];
};

class Circle : public Shape
{
public:
    Circle(double* center, double rad, double* rot);
    virtual ~Circle(){}
    virtual double* getPosition(double*);

private:
    double center[3];
    double radius;
    double temp1[3];
    double temp2[3];
    double rot[9];
    double pos[3];
};

class CircleXY : public Shape
{
public:
    CircleXY(double* center, double rad);
    virtual ~CircleXY(){}
    virtual double* getPosition(double*);

private:
    double center[3];
    double radius;
    double temp1[3];
    double pos[3];
};

class CircleXZ : public Shape
{
public:
    CircleXZ(double* center, double rad);
    virtual ~CircleXZ(){}
    virtual double* getPosition(double*);

private:
    double center[3];
    double radius;
    double temp1[3];
    double pos[3];
};

class CircleYZ : public Shape
{
public:
    CircleYZ(double* center, double rad);
    virtual ~CircleYZ(){}
    virtual double* getPosition(double*);

private:
    double center[3];
    double radius;
    double temp1[3];
    double pos[3];
};

#endif //POSITION_DECOMP_LIBPD_CPP_SHAPES_H
