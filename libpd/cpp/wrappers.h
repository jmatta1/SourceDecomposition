#ifndef POSITION_DECOMP_LIBPD_CPP_WRAPPERS_H
#define POSITION_DECOMP_LIBPD_CPP_WRAPPERS_H

// Make detector shape
extern "C" void* makeDetector(double* vec1, double* vec2, double* norm);

// Make source shapes
extern "C" void* makeSquare(double* cent, double* vec1, double* vec2);
extern "C" void* makeCircle(double* cent, double radius, double* rot);
extern "C" void* makeCircleXY(double* cent, double radius);
extern "C" void* makeCircleXZ(double* cent, double radius);
extern "C" void* makeCircleYZ(double* cent, double radius);

// Function to create and destroy the calculator object
// the calculator object destroys the source and detector objects that it owns
extern "C" void* makeCalculator(void* detector, void* source);
extern "C" void freeCalculator(void* calcObject);

// Function to perform the calculation
extern "C" double calculateIntegrand(int n, double* params, void* calcObject);

#endif //POSITION_DECOMP_LIBPD_CPP_WRAPPERS_H
