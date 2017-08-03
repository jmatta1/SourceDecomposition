#ifndef POSITION_DECOMP_LIBPD_CPP_GEOMETRY_POINTSOURCE_H
#define POSITION_DECOMP_LIBPD_CPP_GEOMETRY_POINTSOURCE_H
#include"Geometry/Shape.h"

class PointSource : public Shape
{
public:
    PointSource(double* cent);
    virtual ~PointSource(){}
    virtual double* getPosition(double* params) override final __attribute__((pure));
    virtual double getVolumeElement(double* params, double* widths) const override final __attribute__((pure));
    virtual int getNumParams() const override final __attribute__((const, pure));
    virtual const double* getBounds() const override final __attribute__((const, pure));

private:
    double pos[3];
};

#endif //POSITION_DECOMP_LIBPD_CPP_GEOMETRY_POINTSOURCE_H
