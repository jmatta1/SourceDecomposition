#ifndef POSITION_DECOMP_LIBPD_CPP_FLAT_SHAPES_H
#define POSITION_DECOMP_LIBPD_CPP_FLAT_SHAPES_H

#include"shape.h"

class Square : public Shape
{
public:
    Square(double* cent, double* v1, double* v2);
    virtual ~Square(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double center[3];
    double vec1[3];
    double mag1;
    double vec2[3];
    double mag2;
    double pos[3];
};

class Circle : public Shape
{
public:
    Circle(double* center, double rad, double* rot);
    virtual ~Circle(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double center[3];
    double radius;
    double rmat[9];
    double pos[3];
};

class CircleXY : public Shape
{
public:
    CircleXY(double* center, double rad);
    virtual ~CircleXY(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double center[3];
    double radius;
    double pos[3];
};

class CircleXZ : public Shape
{
public:
    CircleXZ(double* center, double rad);
    virtual ~CircleXZ(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double center[3];
    double radius;
    double pos[3];
};

class CircleYZ : public Shape
{
public:
    CircleYZ(double* center, double rad);
    virtual ~CircleYZ(){}
    virtual double* getPosition(double* params) override final;
    virtual double getVolumeElement(double* params, double* widths) override final;
    virtual int getNumParams() override final;
    virtual const double* getBounds() override final;

private:
    double center[3];
    double radius;
    double pos[3];
};

#endif //POSITION_DECOMP_LIBPD_CPP_FLAT_SHAPES_H
