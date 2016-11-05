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
        matrix(uint8_t n);
        matrix(uint8_t lines, uint8_t columns);
        matrix(initializer_list<initializer_list<double>> elem);
        ~matrix();

        uint8_t getLines() const;
        uint8_t getColumns() const;
        double** getData() const;

        double& at(uint8_t i, uint8_t j);

        bool operator == (matrix x);
        bool operator != (matrix x);

        matrix operator =(initializer_list<initializer_list<double>> elem);
        matrix operator =(matrix x);
        matrix operator +(matrix x);
        matrix operator +=(matrix x);
        matrix operator -(matrix x);
        matrix operator -=(matrix x);
        matrix operator *(matrix x);
        matrix operator *=(matrix x);
        matrix operator *(double x);
        matrix operator *=(double x);
        matrix operator /(double x);
        matrix operator /=(double x);

        friend matrix operator -(matrix x);
        friend ostream& operator << (ostream& out, matrix x);

        static matrix identity(uint8_t n);
        static matrix transposta(matrix x);
        static matrix rotacao();
    protected:
        double **data;
        uint8_t lines, columns;
};

#endif // MATRIX_H
