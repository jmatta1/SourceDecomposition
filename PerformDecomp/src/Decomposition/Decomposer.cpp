#include"Decomposer.h"
#include<iostream>
#include<sstream>
#include<cmath>

#include<boost/thread.hpp>

namespace Internal
{
static const int DisplayChunk = 1;

static const double ValueThreshold = 1.0e-10;

static const double ConvLimit = 0.005;
}

Decomposer::Decomposer(const ResMatData& respDat, const ScanData& scanDat, int numParallel) :
    numThreads(numParallel), iterationCount(0ULL)
{
    std::tie(respMat, respMatTr, respMatProj, numPositions, numSources) = respDat;
    int placeholder = 0;
    std::tie(scanData, numEnergyBins, placeholder) = scanDat;
    
    newVals = new double[numSources*numThreads];
    oldVals = new double[numSources*numThreads];
    products = new double[numSources*numThreads];
    for(int i=0; i<(numSources*numThreads); ++i)
    {
        newVals[i] = 0.0;
        oldVals[i] = 0.0;
        products[i] = 0.0;
    }
    
    mults = new double[numPositions*numThreads];
    for(int i=0; i<(numPositions*numThreads); ++i)
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
    // fill the queue with the data we need to manage the threads
    std::cout<<"Pushing queue information for threads" << std::endl;
    for(int i=0; i<numEnergyBins; ++i)
    {
        dataQueue.push(i);
    }
    std::cout<<"Creating threads for decomposition"<<std::endl;
    //make the array of callable objects for threads
    DecompThread** threads = new DecompThread*[4];
    for(int i=0; i<numThreads; ++i)
    {
        threads[i] = new DecompThread(this, i);
    }
    //make the thread pool for the threads that will run
    boost::thread_group decompositionThreads;
    for(int i = 0; i < numThreads; ++i)
    {
        decompositionThreads.create_thread(*(threads[i]));
    }
    std::cout << "Waiting for decomposition threads to finish" << std::endl;
    decompositionThreads.join_all();
    std::cout << "Decomposition complete. Total iterations: " << iterationCount.load() << std::endl;
    //delete the callable objects
    for(int i=0; i<numThreads; ++i)
    {
        delete threads[i];
    }
    delete[] threads;
    hasRun = true;
}

bool Decomposer::decompSingleBin(int threadNum)
{
    int index = 0;
    bool gotData = dataQueue.pop(index);
    if(!gotData)
    {
        return false;
    }
    std::ostringstream outBuilder;
    if((index%Internal::DisplayChunk)==0) outBuilder<<"Position decomp on data bin: "<<index+1<<"/"<<numEnergyBins;
    int temp = decomposeSingleBin(index, threadNum);
    iterationCount.fetch_add(temp);
    if((index%Internal::DisplayChunk)==0)
    {
        outBuilder<<"  - Conveged at iteration: "<<temp;
        outBuilder<<"  - Total iterations: "<<iterationCount.load() << std::endl;
        std::cout << outBuilder.str();
    }
    return true;
}

int Decomposer::decomposeSingleBin(int index, int threadNum)
{
    double* data = (scanData+index*numPositions);
    double* newVal = &(newVals[threadNum*numSources]);
    double* oldVal = &(oldVals[threadNum*numSources]);
    double* product = &(products[threadNum*numSources]);
    double* mult = &(mults[threadNum*numPositions]);
    for(int i=0; i<numSources; ++i)
    {
        newVal[i] = 1.0e-10;
        oldVal[i] = outputArray[index*numSources + i];
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
                denom += oldVal[k]*respMatTr[offset+k];
                denom += oldVal[k+1]*respMatTr[offset+k+1];
            }
            mult[j] = data[j]/denom;
        }
        
        //calculate vector resp function multipliers
        for(int j=0; j<numSources; ++j)
        {
            product[j] = 0.0;
            int offset = j*numPositions;
            for(int k=0; k<numPositions; k+=2)
            {
                product[j] += respMat[offset+k]*mult[k];
                product[j] += respMat[offset+k+1]*mult[k+1];
            }
        }
        
        //calculate the new values
        for(int j=0; j<numSources; j+=2)
        {
            newVal[j] = (product[j]*oldVal[j])/respMatProj[j];
            newVal[j+1] = (product[j+1]*oldVal[j+1])/respMatProj[j+1];
        }
        
        //check convergence
        hasConverged = checkConvergence(threadNum);
        //swap new values into old values and vice versa
        double* temp = oldVal;
        oldVal = newVal;
        newVal = temp;
    }
    
    for(int j=0; j<numSources; ++j)
    {
        outputArray[index*numSources + j] = oldVal[j];
    }
    return i;
}

bool Decomposer::checkConvergence(int threadNum)
{
    double* newVal = &(newVals[threadNum*numSources]);
    double* oldVal = &(oldVals[threadNum*numSources]);
    for(int i=0; i<numSources; ++i)
    {
        if(newVal[i] < Internal::ValueThreshold && oldVal[i] < Internal::ValueThreshold)
        {
            continue;
        }
        double frac = std::abs((newVal[i]-oldVal[i])/((oldVal[i]+newVal[i])/2.0));
        if(frac > Internal::ConvLimit)
        {
            return false;
        }
    }
    return true;
}
