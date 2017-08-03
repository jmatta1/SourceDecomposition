#ifndef POSITION_DECOMP_LIBPD_CPP_CINTERFACE_H
#define POSITION_DECOMP_LIBPD_CPP_CINTERFACE_H

// Make detector shape
extern "C" void* makeDetector(double* vec1, double* vec2, double* norm);

//Make Low Dimensional source shapes
extern "C" void* makePoint(double* cent);
extern "C" void* makeLine(double* start, double* stop);

// Make flat source shapes
extern "C" void* makeSquare(double* cent, double* vec1, double* vec2);
extern "C" void* makeCircle(double* cent, double radius, double* rot);
extern "C" void* makeCircleXY(double* cent, double radius);
extern "C" void* makeCircleXZ(double* cent, double radius);
extern "C" void* makeCircleYZ(double* cent, double radius);

//make shell source shapes
extern "C" void* makeVertCylinder(double* cent, double radius, double len);
extern "C" void* makeRotXaxisCylinder(double* cent, double radius, double len, double angle);

// Function to create and destroy the calculator object
// the calculator object destroys the source and detector objects that it owns
extern "C" void* makeCalculator(void* detector, void* source);
extern "C" void freeCalculator(void* calcObject);

// Function to perform the integral calculation
extern "C" void calcIntegral(void* calcObject, double* outParams);

#endif //POSITION_DECOMP_LIBPD_CPP_CINTERFACE_H
