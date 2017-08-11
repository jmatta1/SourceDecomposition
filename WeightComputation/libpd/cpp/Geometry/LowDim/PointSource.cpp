#include"PointSource.h"

static const double PointBounds[1] = {0.0};

PointSource::PointSource(double* cent)
{
    for(int i=0; i<3; ++i)
    {
        pos[i] = cent[i];
    }
}

double* PointSource::getPosition(double* params)
{
    return pos;
}

double PointSource::getVolumeElement(double* params, double* widths) const
{
    return 1.0;
}

int PointSource::getNumParams() const
{
    return 0;
}

const double* PointSource::getBounds() const
{
    return PointBounds;
}
