#ifndef POSITION_DECOMP_LIBPD_CPP_GEOMETRY_PLANARCIRCLES_H
#define POSITION_DECOMP_LIBPD_CPP_GEOMETRY_PLANARCIRCLES_H

#include"Shape.h"

class CircleXY : public Shape
{
public:
    CircleXY(double* center, double rad);
    virtual ~CircleXY(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double radius;
    double radSquare;
    double center[3];
    double pos[3];
};

class CircleXZ : public Shape
{
public:
    CircleXZ(double* center, double rad);
    virtual ~CircleXZ(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double radius;
    double radSquare;
    double center[3];
    double pos[3];
};

class CircleYZ : public Shape
{
public:
    CircleYZ(double* center, double rad);
    virtual ~CircleYZ(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double radius;
    double radSquare;
    double center[3];
    double pos[3];
};

#endif //POSITION_DECOMP_LIBPD_CPP_GEOMETRY_PLANARCIRCLES_H
