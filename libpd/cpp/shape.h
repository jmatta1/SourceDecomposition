#ifndef POSITION_DECOMP_LIBPD_CPP_SHAPE_H
#define POSITION_DECOMP_LIBPD_CPP_SHAPE_H

// the abstract base class for the various shape classes
class Shape
{
public:
    virtual ~Shape(){}
    virtual double* getPosition(double*) = 0;
};

#endif //POSITION_DECOMP_LIBPD_CPP_SHAPE_H
