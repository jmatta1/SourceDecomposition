#include"detector.h"

Detector::Detector(double* v1, double* v2, double* nm)
{
    for(int i=0; i<3; ++i)
    {
        vec1[i] = v1[i];
        vec2[i] = v2[i];
        norm[i] = nm[i];
    }
}

double* Detector::getPosition(double a, double b)
{
    pos[0] = a*vec1[0] + b*vec2[0];
    pos[1] = a*vec1[1] + b*vec2[1];
    pos[2] = a*vec1[2] + b*vec2[2];
}

bool Detector::hasLineOfSight(double* pos)
{
    double dotProd = 0.0;
    for(int i=0; i<3; ++i)
    {
        dotProd += (pos[i]*norm[i]);
    }
    return (dotProd > 0.0);
}
