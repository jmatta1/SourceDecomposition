#ifndef POSITION_DECOMP_LIBPD_CPP_GEOMETRY_DETECTOR_H
#define POSITION_DECOMP_LIBPD_CPP_GEOMETRY_DETECTOR_H

static const double DetBounds[4] = {-1.0, 1.0, -1.0, 1.0};

// the class for a detector surface
class Detector
{
public:
    Detector(double* v1, double* v2, double* nm);
    double* getPosition(double* params) __attribute__((pure));
    double getSurfaceElement(double* params, double* widths) const __attribute__((pure));
    int getNumParams() const __attribute__((pure)) {return 2;}
    const double* getBounds() const __attribute__((const, pure)) {return DetBounds;}
    bool hasLineOfSight(double* posVec) const __attribute__((const, pure));

private:
    double vec1[3];
    double mag1;
    double vec2[3];
    double mag2;
    double norm[3];
    double pos[3];
};

#endif //POSITION_DECOMP_LIBPD_CPP_DETECTOR_H
