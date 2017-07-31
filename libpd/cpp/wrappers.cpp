#include"wrappers.h"
#include"detector.h"
#include"flat_shapes.h"
#include"shell_shapes.h"
#include"calculator.h"

void* makeDetector(double* vec1, double* vec2, double* norm)
{
    Detector* temp = new Detector(vec1, vec2, norm);
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
    VertCylinder* temp = new VertCylinder(cent, radius, len);
    return (void*)temp;
}

void* makeRotXaxisCylinder(double* cent, double radius, double len, double angle)
{
    RotXaxisCylinder* temp = new RotXaxisCylinder(cent, radius, len, angle);
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
    for(int i=0; i<3; ++i)
    {
        outParams[i] = temp[i];
    }
}
