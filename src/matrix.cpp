#include "matrix.h"

matrix::matrix()
{
    this->lines=this->columns=1;
    data= new float*;
    data[0]= new float;
}
matrix::matrix(int n)
{
    this->lines=this->columns=n;
    data= new float*[n];
    for(int i=0; i<n; i++)
        data[i]= new float[n];
}
matrix::matrix(int lines, int columns)
{
    this->lines=lines;
    this->columns=columns;
    data= new float*[lines];
    for(int i=0; i<lines; i++)
        data[i]= new float[columns];
}
float matrix::getLines() const
{
    return lines;
}
float matrix::getColumns() const
{
    return columns;
}
float** matrix::getData() const
{
    return data;
}
float& matrix::at(int i, int j)
{
    if(i >= 0 && i < this->lines && j >= 0 && j < this->columns)
        return this->data[i][j];
    throw "Erro de acesso matrix";
}
bool matrix::operator == (matrix x)
{
    if(this->lines != x.lines || this->columns != x.columns)
        return false;
    for(int i=0; i<x.lines; i++)
        for(int j=0; j<x.columns; j++)
            if(this->data[i][j] != x.data[i][j])
                return false;
    return true;
}
bool matrix::operator !=(matrix x)
{
    return !(*this==x);
}
matrix matrix::operator =(initializer_list<initializer_list<float>> elem)
{
    bool fimL= false, fimC= false;
    for(int i=0; i<this->lines; i++)
    {
        if((elem.begin()+i) == elem.end())
            fimL= true;
        if(fimL)
        {
            for(int j=0; j<this->columns; j++)
                this->data[i][j]= 0;
        }
        else
        {
            for(int j=0; j<this->columns; j++)
            {
                if(((elem.begin()+i)->begin()+j) == (elem.begin()+i)->end())
                    fimC= true;
                if(fimC)
                    this->data[i][j]= 0;
                else
                    this->data[i][j]= *((elem.begin()+i)->begin()+j);
            }
        }
        fimC= false;
    }
    return *this;
}
matrix matrix::operator =(matrix x)
{
    delete[] data;
    lines= x.lines;
    columns= x.columns;
    data= new float*[lines];
    for(int i=0; i<lines; i++)
        data[i]= new float[columns];
    for(int i=0; i<lines; i++)
        for(int j=0; j<columns; j++)
            data[i][j]= x.data[i][j];
    return *this;
}
matrix matrix::operator +(matrix x)
{
    if(x.columns != this->columns || x.lines != this->lines)
        throw "Operacao Invalida matrix soma";
    matrix ret(this->lines, this->columns);
    for(int i=0; i<this->lines; i++)
        for(int j=0; j<this->columns; j++)
            ret.data[i][j]= this->data[i][j]+x.data[i][j];
    return ret;
}
matrix matrix::operator +=(matrix x)
{
    return *this= *this+x;
}
matrix matrix::operator -(matrix x)
{
    if(x.columns != this->columns || x.lines != this->lines)
        throw "Operacao Invalida matrix subtracao";
    matrix ret(this->lines, this->columns);
    for(int i=0; i<this->lines; i++)
        for(int j=0; j<this->columns; j++)
            ret.data[i][j]= this->data[i][j]-x.data[i][j];
    return ret;
}
matrix matrix::operator -=(matrix x)
{
    return *this= *this-x;
}
matrix matrix::operator *(matrix x)
{
    if(this->columns != x.lines)
        throw "Operacao Invalida matrix multiplicacao";
    matrix ret(this->lines, x.columns);
    for(int i=0; i<this->lines; i++)
        for(int j=0; j<x.columns; j++)
            {
                ret.data[i][j]=0;
                for(int k=0; k<this->columns; k++)
                    ret.data[i][j]+= this->data[i][k]*x.data[k][j];
            }
    return ret;
}
matrix matrix::operator *=(matrix x)
{
    return *this= *this*x;
}
matrix matrix::operator *(float x)
{
    matrix ret(this->lines, this->columns);
    for(int i=0; i<this->lines; i++)
        for(int j=0; j<this->columns; j++)
            ret.data[i][j]= this->data[i][j]*x;
    return ret;
}
matrix matrix::operator *=(float x)
{
    return *this= *this*x;
}
matrix matrix::operator /(float x)
{
    if(x == 0)
        throw "Erro matrix div/0";
    matrix ret(this->lines, this->columns);
    for(int i=0; i<this->lines; i++)
        for(int j=0; j<this->columns; j++)
            ret.data[i][j]= this->data[i][j]/x;
    return ret;
}
matrix matrix::operator /=(float x)
{
    return *this= *this/x;
}
ostream& operator << (ostream& out, matrix x)
{
    for(int i=0; i<x.lines; i++)
    {
        for(int j=0; j<x.columns; j++)
        {
            out << x.data[i][j] << " ";
        }
        out << endl;
    }
    return out;
}

matrix matrix::identity(int n)
{
    matrix ret(n);
    for(int i=0; i<n; i++)
        ret.data[i][i]= 1;
    return ret;
}
matrix matrix::transposta(matrix x)
{
    matrix ret(x.columns, x.lines);
    for(int i=0; i<x.lines; i++)
        for(int j=0; j<x.columns; j++)
            ret.data[j][i]= x.data[i][j];
    return ret;
}