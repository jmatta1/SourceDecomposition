#ifndef POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DCOMPLEX_AXIALCYLINDER_H
#define POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DCOMPLEX_AXIALCYLINDER_H

#include"Geometry/Shape.h"

class CylinderXaxis : public Shape
{
public:
    CylinderXaxis(double* cent, double rad, double len);
    virtual ~CylinderXaxis(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double center[3];
    double radius;
    double length;
    double areaScale;
    double pos[3];
};



class CylinderYaxis : public Shape
{
public:
    CylinderYaxis(double* cent, double rad, double len);
    virtual ~CylinderYaxis(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double center[3];
    double radius;
    double length;
    double areaScale;
    double pos[3];
};



class CylinderZaxis : public Shape
{
public:
    CylinderZaxis(double* cent, double rad, double len);
    virtual ~CylinderZaxis(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double center[3];
    double radius;
    double length;
    double areaScale;
    double pos[3];
};

#endif //POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DCOMPLEX_AXIALCYLINDER_H
