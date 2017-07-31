#ifndef POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DFLAT_CIRCLE_H
#define POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DFLAT_CIRCLE_H

#include"Geometry/Shape.h"
class Circle : public Shape
{
public:
    Circle(double* center, double rad, double* rot);
    virtual ~Circle(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double radius;
    double radSquare;
    double center[3];
    double rmat[9];
    double pos[3];
};

#endif //POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DFLAT_CIRCLE_H
