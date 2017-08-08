#include"Decomposer.h"
#include<iostream>
#include<cmath>

Decomposer::Decomposer(const ResMatData& respDat, const ScanData& scanDat)
{
    std::tie(respMat, respMatTr, respMatProj, numPositions, numSources) = respDat;
    int placeholder = 0;
    std::tie(scanData, numEnergyBins, placeholder) = scanDat;
    
    newVals = new double[numSources];
    oldVals = new double[numSources];
    products = new double[numSources];
    for(int i=0; i<numSources; ++i)
    {
        newVals[i] = 0.0;
        oldVals[i] = 0.0;
        products[i] = 0.0;
    }
    
    mults = new double[numPositions];
    for(int i=0; i<numPositions; ++i)
    {
        mults[i] = 0.0;
    }
    
    placeholder = numSources*numEnergyBins;
    outputArray = new double[placeholder];
    for(int i=0; i<placeholder; ++i)
    {
        outputArray[i] = 1.0;
    }
}

void Decomposer::performDecomp()
{
    // to do the serial decomposition we need to iterate over the data bins
    for(int i=0; i<numEnergyBins; ++i)
    {
        if((i%Internal::DisplayChunk)==0) std::cout<<"Position decomp on data bin: "<<i+1<<"/"<<numEnergyBins;
        int temp = decomposeSingleBin(i);
        if((i%Internal::DisplayChunk)==0) std::cout<<"  - Conveged at iteration: "<<temp<<std::endl;
    }
    hasRun = true;
}

int Decomposer::decomposeSingleBin(int index)
{
    double* data = (scanData+index*numPositions);
    for(int i=0; i<numSources; ++i)
    {
        newVals[i] = 1.0e-10;
        oldVals[i] = outputArray[index*numSources + i];
    }
    
    unsigned long long i = 0;
    bool hasConverged = false;
    for(; !hasConverged; ++i)
    {
        //calculate multipliers
        for(int j=0; j<numPositions; ++j)
        {
            double denom = 0.0;
            int offset = j*numSources;
            for(int k=0; k<numSources; k+=2)
            {
                denom += oldVals[k]*respMatTr[offset+k];
                denom += oldVals[k+1]*respMatTr[offset+k+1];
            }
            mults[j] = data[j]/denom;
        }
        
        //calculate vector resp function multipliers
        for(int j=0; j<numSources; ++j)
        {
            products[j] = 0.0;
            int offset = j*numPositions;
            for(int k=0; k<numPositions; k+=2)
            {
                products[j] += respMat[offset+k]*mults[k];
                products[j] += respMat[offset+k+1]*mults[k+1];
            }
        }
        
        //calculate the new values
        for(int j=0; j<numSources; j+=2)
        {
            newVals[j] = (products[j]*oldVals[j])/respMatProj[j];
            newVals[j+1] = (products[j+1]*oldVals[j+1])/respMatProj[j+1];
        }
        
        //check convergence
        hasConverged = checkConvergence();
        //swap new values into old values and vice versa
        double* temp = oldVals;
        oldVals = newVals;
        newVals = temp;
    }
    
    for(int j=0; j<numSources; ++j)
    {
        outputArray[index*numSources + j] = oldVals[j];
    }
    return i;
}

bool Decomposer::checkConvergence()
{
    for(int i=0; i<numSources; ++i)
    {
        if(newVals[i] < Internal::ValueThreshold && oldVals[i] < Internal::ValueThreshold)
        {
            continue;
        }
        double frac = std::abs((newVals[i]-oldVals[i])/((oldVals[i]+newVals[i])/2.0));
        if(frac > Internal::ConvLimit)
        {
            return false;
        }
    }
    return true;
}
