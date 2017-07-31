#ifndef POSITION_DECOMP_LIBPD_CPP_SHAPE_H
#define POSITION_DECOMP_LIBPD_CPP_SHAPE_H

// the abstract base class for the various shape classes
class Shape
{
public:
    virtual ~Shape(){}
    virtual double* getPosition(double* params) = 0;
    virtual double getVolumeElement(double* params, double* widths) = 0;
    virtual int getNumParams() = 0;
    virtual const double* getBounds() = 0;
};

#endif //POSITION_DECOMP_LIBPD_CPP_SHAPE_H
