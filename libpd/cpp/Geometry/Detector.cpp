#include"Detector.h"
#include<cmath>
#include<iostream>
using std::cout;
using std::endl;

Detector::Detector(double* v1, double* v2, double* nm) : mag1(0.0), mag2(0.0)
{
    mag1=0.0;
    mag2=0.0;
    for(int i=0; i<3; ++i)
    {
        vec1[i] = v1[i];
        mag1 += (v1[i]*v1[i]);
        vec2[i] = v2[i];
        mag2 += (v2[i]*v2[i]);
        norm[i] = nm[i];
    }
    mag1 = std::sqrt(mag1);
    mag2 = std::sqrt(mag2);
}

double* Detector::getPosition(double* params)
{
    pos[0] = params[0]*vec1[0] + params[1]*vec2[0];
    pos[1] = params[0]*vec1[1] + params[1]*vec2[1];
    pos[2] = params[0]*vec1[2] + params[1]*vec2[2];
    return pos;
}

double Detector::getSurfaceElement(double* params, double* widths)
{
    return std::abs(widths[0]*mag1 * widths[1]*mag2);
}

bool Detector::hasLineOfSight(double* posVec)
{
    double dotProd = 0.0;
    for(int i=0; i<3; ++i)
    {
        dotProd += (posVec[i]*norm[i]);
    }
    return (dotProd > 0.0);
}
