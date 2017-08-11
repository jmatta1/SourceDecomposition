#include"LineSource.h"
#include<cmath>

static const double LineBounds[2] = {0.0, 1.0};

LineSource::LineSource(double* start, double* stop)
{
    double temp = 0.0;
    for(int i=0; i<3; ++i)
    {
        pt1[i] = start[i];
        vec1[i] = stop[i];
        temp += ((stop[i] - start[i])*(stop[i] - start[i]));
    }
    mag = std::sqrt(temp);
}

double* LineSource::getPosition(double* params)
{
    pos[0] = pt1[0] + params[0]*vec1[0];
    pos[1] = pt1[1] + params[0]*vec1[1];
    pos[2] = pt1[2] + params[0]*vec1[2];
    return pos;
}

double LineSource::getVolumeElement(double* params, double* widths) const
{
    return mag*widths[0];
}

int LineSource::getNumParams() const
{
    return 1;
}

const double* LineSource::getBounds() const
{
    return LineBounds;
}
