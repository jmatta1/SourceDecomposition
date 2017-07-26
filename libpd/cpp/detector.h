#ifndef POSITION_DECOMP_LIBPD_CPP_DETECTOR_H
#define POSITION_DECOMP_LIBPD_CPP_DETECTOR_H

// the class for a detector surface
class Detector
{
public:
    double* getPosition(double a, double b);
    bool hasLineOfSight(double* pos);
    
private:
    double detVec1[3];
    double detVec2[3];
    double norm[3]
};
#endif //POSITION_DECOMP_LIBPD_CPP_DETECTOR_H
