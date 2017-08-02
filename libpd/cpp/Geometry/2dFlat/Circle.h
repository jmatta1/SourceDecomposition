#ifndef POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DFLAT_CIRCLE_H
#define POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DFLAT_CIRCLE_H

#include"Geometry/Shape.h"
class Circle : public Shape
{
public:
    Circle(double* center, double rad, double* rot);
    virtual ~Circle(){}
    virtual double* getPosition(double* params) override final __attribute__((pure));
    virtual double getVolumeElement(double* params, double* widths) const override final __attribute__((pure));
    virtual int getNumParams() const override final __attribute__((const, pure));
    virtual const double* getBounds() const override final __attribute__((const, pure));

private:
    double radius;
    double radSquare;
    double center[3];
    double rmat[9];
    double pos[3];
};

#endif //POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DFLAT_CIRCLE_H
