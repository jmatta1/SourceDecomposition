#include"CInterface.h"
#include"Geometry/Detector.h"
#include"Geometry/LowDimSources.h"
#include"Geometry/FlatShapes.h"
#include"Geometry/ShellShapes.h"
#include"Calculation/Calculator.h"

void* makeDetector(double* vec1, double* vec2, double* norm)
{
    Detector* temp = new Detector(vec1, vec2, norm);
    return (void*)temp;
}

//Make Point source
void* makePoint(double* cent)
{
    PointSource* temp = new PointSource(cent);
    return (void*)temp;
}

//Make Line source
void* makeLine(double* start, double* stop)
{
    LineSource* temp = new LineSource(start, stop);
    return (void*)temp;
}

// Make 2D source shapes
void* makeSquare(double* cent, double* vec1, double* vec2)
{
    Square* temp = new Square(cent, vec1, vec2);
    return (void*)temp;
}

void* makeCircle(double* cent, double radius, double* rot)
{
    Circle* temp = new Circle(cent, radius, rot);
    return (void*)temp;
}

void* makeCircleXY(double* cent, double radius)
{
    CircleXY* temp = new CircleXY(cent, radius);
    return (void*)temp;
}

void* makeCircleXZ(double* cent, double radius)
{
    CircleXZ* temp = new CircleXZ(cent, radius);
    return (void*)temp;
}

void* makeCircleYZ(double* cent, double radius)
{
    CircleYZ* temp = new CircleYZ(cent, radius);
    return (void*)temp;
}

void* makeVertCylinder(double* cent, double radius, double len)
{
    CylinderZaxis* temp = new CylinderZaxis(cent, radius, len);
    return (void*)temp;
}

void* makeRotXaxisCylinder(double* cent, double radius, double len, double angle)
{
    CylinderXaxisRotatedZ* temp = new CylinderXaxisRotatedZ(cent, radius, len, angle);
    return (void*)temp;
}


// Function to create the calculator object
void* makeCalculator(void* detector, void* source)
{
    Calculator* temp = new Calculator((Detector*)detector, (Shape*)source);
    return (void*)temp;
}

void freeCalculator(void* calcObject)
{
    Calculator* temp = (Calculator*)calcObject;
    delete temp;
}


// Function to perform the calculation
void calcIntegral(void* calcObject, double* outParams)
{
    double* temp = ((Calculator*)calcObject)->calcIntegral();
    for(int i=0; i<5; ++i)
    {
        outParams[i] = temp[i];
    }
}
