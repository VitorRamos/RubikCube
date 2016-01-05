#ifndef VEC3_H
#define VEC3_H

#pragma once

#include <math.h>
#include "matrix.h"

class vec3 : public matrix
{
public:
    vec3();
    vec3(double x, double y, double z);
    vec3(matrix x);
    vec3 operator =(matrix x);
    void set(double x, double y, double z);
    double x();
    double y();
    double z();
    double& at(int i);
    double norma();
    double produto(vec3 x);
    vec3 cruzado(vec3 x);
    vec3 projecao(vec3 x);
    static vec3 normalizar(vec3 x);

    friend ostream& operator << (ostream& out, vec3 x);
};
#endif // VEC3_H
