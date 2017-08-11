#ifndef POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DCOMPLEX_SIMPLECYLINDER_H
#define POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DCOMPLEX_SIMPLECYLINDER_H

#include"Geometry/Shape.h"

class CylinderXaxisRotatedZ : public Shape
{
public:
    CylinderXaxisRotatedZ(double* cent, double rad, double len, double angle);
    virtual ~CylinderXaxisRotatedZ(){}
    virtual double* getPosition(double* params) override final __attribute__((pure));
    virtual double getVolumeElement(double* params, double* widths) const override final __attribute__((pure));
    virtual int getNumParams() const override final __attribute__((const, pure));
    virtual const double* getBounds() const override final __attribute__((const, pure));

private:
    double center[3];
    double radius;
    double length;
    double areaScale;
    double sEval;
    double cEval;
    double pos[3];
};

#endif //POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DCOMPLEX_SIMPLECYLINDER_H
