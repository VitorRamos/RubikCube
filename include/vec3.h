#ifndef VEC3_H
#define VEC3_H

#pragma once

#include <math.h>
#include "matrix.h"

class vec3 : public matrix
{
public:
    vec3();
    vec3(float x, float y, float z);
    vec3(matrix x);
    vec3 operator =(matrix x);
    void set(float x, float y, float z);
    float x();
    float y();
    float z();
    float& at(int i);
    float norma();
    float produto(vec3 x);
    vec3 cruzado(vec3 x);
    vec3 projecao(vec3 x);
    static vec3 normalizar(vec3 x);

    friend ostream& operator << (ostream& out, vec3 x);
};
#endif // VEC3_H
