#ifndef POSITION_DECOMP_LIBPD_CPP_GEOMETRY_SHAPE_H
#define POSITION_DECOMP_LIBPD_CPP_GEOMETRY_SHAPE_H

// the abstract base class for the various shape classes
class Shape
{
public:
    virtual ~Shape(){}
    virtual double* getPosition(double* params) __attribute__((pure)) = 0;
    virtual double getVolumeElement(double* params, double* widths) const __attribute__((pure)) = 0;
    virtual int getNumParams() const __attribute__((const, pure)) = 0;
    virtual const double* getBounds() const __attribute__((const, pure)) = 0;
};

#endif //POSITION_DECOMP_LIBPD_CPP_GEOMETRY_SHAPE_H
