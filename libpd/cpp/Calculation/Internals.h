#ifndef POSITION_DECOMP_LIBPD_CPP_CALCULATION_INTERNALS_H
#define POSITION_DECOMP_LIBPD_CPP_CALCULATION_INTERNALS_H

namespace Internal
{
static const double InvFourPi = 0.079577471545947667884441881686257181;
static const int MinDepth = 4;
static const int MaxDepth = 60;
static const double ConvergenceLimit = 1.0e-5;
static const double ValueLimit = 1.0e-14;

//indexes into this table need to be numParams - 2
//the maximum number of parameters is 9 by this table, but it should really be much less
//For any reasonable 3D object source the number of params should only be 5 total,
// 2 for detector, 3 for source. It may reach as high as 6 if one gets fancy with their detectors and makes them 3D
static const int ValueAllocationTable[8] = {4, 12, 32, 80, 240, 672, 1792, 5376};
static const int DiffAllocationTable[8] = {2, 3, 6, 10, 20, 35, 70, 126};


}

#endif //POSITION_DECOMP_LIBPD_CPP_CALCULATION_INTERNALS_H

