#ifndef POSITION_DECOMP_LIBPD_CPP_CALCULATOR_H
#define POSITION_DECOMP_LIBPD_CPP_CALCULATOR_H
#include"detector.h"
#include"shape.h"

class Calculator
{
public:
    Calculator(Detector* d, Shape* s):src(s), det(d){}
    ~Calculator(){delete src; delete det;}
    
    double calcIntegrand(double* params);

private:
    Shape* src;
    Detector* det;
};

#endif //POSITION_DECOMP_LIBPD_CPP_CALCULATOR_H
