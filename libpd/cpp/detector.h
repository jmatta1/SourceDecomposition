#ifndef POSITION_DECOMP_LIBPD_CPP_DETECTOR_H
#define POSITION_DECOMP_LIBPD_CPP_DETECTOR_H

// the class for a detector surface
class Detector
{
public:
    Detector(double* v1, double* v2, double* nm);
    double* getPosition(double a, double b);
    bool hasLineOfSight(double* pos);
    
private:
    double vec1[3];
    double vec2[3];
    double norm[3];
    double pos[3];
};
#endif //POSITION_DECOMP_LIBPD_CPP_DETECTOR_H
