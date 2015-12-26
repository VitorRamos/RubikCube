#ifndef MATRIX_H
#define MATRIX_H

#pragma once
#include <ostream>
#include <initializer_list>

using std::ostream;
using std::endl;
using std::initializer_list;

class matrix
{
    public:
        matrix();
        matrix(int n);
        matrix(int lines, int columns);

        float getLines() const;
        float getColumns() const;
        float** getData() const;

        float& at(int i, int j);

        bool operator == (matrix x);
        bool operator != (matrix x);

        matrix operator =(initializer_list<initializer_list<float>> elem);
        matrix operator =(matrix x);
        matrix operator +(matrix x);
        matrix operator +=(matrix x);
        matrix operator -(matrix x);
        matrix operator -=(matrix x);
        matrix operator *(matrix x);
        matrix operator *=(matrix x);
        matrix operator *(float x);
        matrix operator *=(float x);
        matrix operator /(float x);
        matrix operator /=(float x);

        friend ostream& operator << (ostream& out, matrix x);

        static matrix identity(int n);
        static matrix transposta(matrix x);
    protected:
        float **data;
        int lines, columns;
};

#endif // MATRIX_H
