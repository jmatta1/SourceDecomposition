#include"calculator.h"

static const double InvFourPi = 0.079577471545947667884441881686257181;

double Calculator::calcIntegrand(double* params)
{
    double* srcPos = src->getPosition(params+2);
    if(det->hasLineOfSight(srcPos))
    {
        double* detPos = det->getPosition(params[0], params[1]);
        double dist = 0.0;
        for(int i=0; i<3; ++i)
        {
            double temp = srcPos[i] - detPos[i];
            dist += (temp * temp);
        }
        return InvFourPi/dist;
    }
    else
    {
        return 0.0;
    }
}
