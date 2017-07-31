#ifndef POSITION_DECOMP_LIBPD_CPP_GEOMETRY_SHELLSHAPES_H
#define POSITION_DECOMP_LIBPD_CPP_GEOMETRY_SHELLSHAPES_H

#include"Shape.h"

class VertCylinder : public Shape
{
public:
    VertCylinder(double* cent, double rad, double len);
    virtual ~VertCylinder(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double center[3];
    double radius;
    double length;
    double pos[3];
};

class RotXaxisCylinder : public Shape
{
public:
    RotXaxisCylinder(double* cent, double rad, double len, double angle);
    virtual ~RotXaxisCylinder(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double center[3];
    double radius;
    double length;
    double sEval;
    double cEval;
    double pos[3];
};

#endif //POSITION_DECOMP_LIBPD_CPP_SHELL_SHAPES_H
