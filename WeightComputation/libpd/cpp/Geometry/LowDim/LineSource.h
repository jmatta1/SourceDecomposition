#ifndef POSITION_DECOMP_LIBPD_CPP_GEOMETRY_LINESOURCE_H
#define POSITION_DECOMP_LIBPD_CPP_GEOMETRY_LINESOURCE_H
#include"Geometry/Shape.h"

class LineSource : public Shape
{
public:
    LineSource(double* start, double* stop);
    virtual ~LineSource(){}
    virtual double* getPosition(double* params) override final __attribute__((pure));
    virtual double getVolumeElement(double* params, double* widths) const override final __attribute__((pure));
    virtual int getNumParams() const override final __attribute__((const, pure));
    virtual const double* getBounds() const override final __attribute__((const, pure));

private:
    double mag = 0.0;
    double pt1[3];
    double vec1[3];
    double pos[3];
};

#endif //POSITION_DECOMP_LIBPD_CPP_GEOMETRY_LINESOURCE_H
