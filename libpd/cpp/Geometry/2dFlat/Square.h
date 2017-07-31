#ifndef POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DFLAT_SQUARE_H
#define POSITION_DECOMP_LIBPD_CPP_GEOMETRY_2DFLAT_SQUARE_H

#include"Geometry/Shape.h"

class Square : public Shape
{
public:
    Square(double* cent, double* v1, double* v2);
    virtual ~Square(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double center[3];
    double vec1[3];
    double mag1;
    double vec2[3];
    double mag2;
    double pos[3];
};

#endif // POSITION_DECOMP_LIBPD_CPP_GEOMETRY_SQUARE_H
