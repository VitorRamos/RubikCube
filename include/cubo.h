#ifndef CUBO_H
#define CUBO_H

#pragma once
#include <GL/gl.h>
#include <GL/glut.h>
#include <string>
#include <vector>

#include "utilitarios.h"
#include "vec3.h"

using std::vector;
using std::string;

class face
{
    vec3 pos;
    vec3 normal;
    matrix square;
    string cor;
public:
    face(vec3 pos, vec3 normal, string cor);
    void Draw();
    void AplicaTransformacao(matrix T);
    vec3 getPos() const;
    vec3 getNormal() const;
    string getCor() const;
};

class cubiculo
{
    friend class Cubo;
    vec3 pos;
    vector<face> faces;
public:
    cubiculo(vec3 pos, vector<face> faces);
    void Draw();
    void AplicaTransformacao(matrix T);
    face ProcuraFace(string cor);
    vector<face> getFaces() const;
    vec3 getPos() const;
};

class Cubo
{
    vector<cubiculo*> frontFace, backFace, leftFace, rightFace, middleFace1, middleFace2, middleFace3, upFace, downFace;
    vector<cubiculo> all;
public:
    Cubo();
    void operator= (Cubo x)
    {
        all= x.all;
        UpDate();
    }
    void AplicaT(string nomeF, matrix T);
    void Draw();
    vector<cubiculo> ProcuraPeca(string cor, unsigned int numFace);
    cubiculo ProcuraPeca(vec3 pos);
    void AplicaJogada(vector<string> jogada);
private:
    void UpDate();
};

#endif // CUBO_H
