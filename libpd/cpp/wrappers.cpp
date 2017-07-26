static const double invFourPi = 0.079577471545947667884441881686257181;

extern "C" double evaluate(int, double*);

double evaluate(int n, double* paramArray)
{
    //six parameters, (X0, Y0, Z0) and (X1, Y1, Z1)
    //calculate the differences of those parameters
    double diff0 = (paramArray[3] - paramArray[0]);
    double diff1 = (paramArray[4] - paramArray[1]);
    double diff2 = (paramArray[5] - paramArray[2]);
    double rsq = diff0*diff0 + diff1*diff1 + diff2*diff2;
    return (invFourPi/rsq);
}
