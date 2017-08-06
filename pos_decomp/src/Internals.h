#ifndef POSITIONDECOMP_INTERNALS_H
#define POSITIONDECOMP_INTERNALS_H

#include<string>

namespace Internal
{
static const int DisplayChunk = 50;

static const double ValueThreshold = 1.0e-7;

static const double ConvLimit = 0.005;

static const char* SourceNames[14] = {"Front_Bottom_Right_Corner",
"Front_Top_Left_Corner", "Front_Bottom_Right_Corner", "Front_Bottom_Left_Corner",
"Back_Top_Right_Corner", "Back_Top_Left_Corner", "Back_Bottom_Right_Corner",
"Back_Bottom_Left_Corner", "EF4_Cover", "HB3_Access", "HB4_Access", "HB3",
"HB3a", "HB4"};

}

#endif //POSITIONDECOMP_INTERNALS_H
