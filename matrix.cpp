#include "matrix.h"

matrix::matrix()
{
    this->lines=this->columns=1;
    data= new double*;
    data[0]= new double;
}
matrix::matrix(uint8_t n)
{
    this->lines=this->columns=n;
    data= new double*[n];
    for(uint8_t i=0; i<n; i++)
        data[i]= new double[n];
}
matrix::matrix(uint8_t lines, uint8_t columns)
{
    this->lines=lines;
    this->columns=columns;
    data= new double*[lines];
    for(uint8_t i=0; i<lines; i++)
        data[i]= new double[columns];
}
matrix::matrix(initializer_list<initializer_list<double>> elem)
{
    this->lines= elem.size();
    this->columns= elem.begin()->size();
    data= new double*[lines];
    for(uint8_t i=0; i<lines; i++)
        data[i]= new double[columns];
    bool fimL= false, fimC= false;
    for(uint8_t i=0; i<this->lines; i++)
    {
        if((elem.begin()+i) == elem.end())
            fimL= true;
        if(fimL)
        {
            for(uint8_t j=0; j<this->columns; j++)
                this->data[i][j]= 0;
        }
        else
        {
            for(uint8_t j=0; j<this->columns; j++)
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
}
matrix::~matrix()
{
}
uint8_t matrix::getLines() const
{
    return lines;
}
uint8_t matrix::getColumns() const
{
    return columns;
}
double** matrix::getData() const
{
    return data;
}
double& matrix::at(uint8_t i, uint8_t j)
{
    if(i >= 0 && i < this->lines && j >= 0 && j < this->columns)
        return this->data[i][j];
    throw "Erro de acesso matrix";
}
bool matrix::operator == (matrix x)
{
    if(this->lines != x.lines || this->columns != x.columns)
        return false;
    for(uint8_t i=0; i<x.lines; i++)
        for(uint8_t j=0; j<x.columns; j++)
            if(this->data[i][j] != x.data[i][j])
                return false;
    return true;
}
bool matrix::operator !=(matrix x)
{
    return !(*this==x);
}
matrix matrix::operator =(initializer_list<initializer_list<double>> elem)
{
    this->lines= elem.size();
    this->columns= elem.begin()->size();
    data= new double*[lines];
    for(uint8_t i=0; i<lines; i++)
        data[i]= new double[columns];
    bool fimL= false, fimC= false;
    for(uint8_t i=0; i<this->lines; i++)
    {
        if((elem.begin()+i) == elem.end())
            fimL= true;
        if(fimL)
        {
            for(uint8_t j=0; j<this->columns; j++)
                this->data[i][j]= 0;
        }
        else
        {
            for(uint8_t j=0; j<this->columns; j++)
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
    data= new double*[lines];
    for(uint8_t i=0; i<lines; i++)
        data[i]= new double[columns];
    for(uint8_t i=0; i<lines; i++)
        for(uint8_t j=0; j<columns; j++)
            data[i][j]= x.data[i][j];
    return *this;
}
matrix matrix::operator +(matrix x)
{
    if(x.columns != this->columns || x.lines != this->lines)
        throw "Operacao Invalida matrix soma";
    matrix ret(this->lines, this->columns);
    for(uint8_t i=0; i<this->lines; i++)
        for(uint8_t j=0; j<this->columns; j++)
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
    for(uint8_t i=0; i<this->lines; i++)
        for(uint8_t j=0; j<this->columns; j++)
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
    for(uint8_t i=0; i<this->lines; i++)
        for(uint8_t j=0; j<x.columns; j++)
            {
                ret.data[i][j]=0;
                for(uint8_t k=0; k<this->columns; k++)
                    ret.data[i][j]+= this->data[i][k]*x.data[k][j];
            }
    return ret;
}
matrix matrix::operator *=(matrix x)
{
    return *this= *this*x;
}
matrix matrix::operator *(double x)
{
    matrix ret(this->lines, this->columns);
    for(uint8_t i=0; i<this->lines; i++)
        for(uint8_t j=0; j<this->columns; j++)
            ret.data[i][j]= this->data[i][j]*x;
    return ret;
}
matrix matrix::operator *=(double x)
{
    return *this= *this*x;
}
matrix matrix::operator /(double x)
{
    if(x == 0)
        throw "Erro matrix div/0";
    matrix ret(this->lines, this->columns);
    for(uint8_t i=0; i<this->lines; i++)
        for(uint8_t j=0; j<this->columns; j++)
            ret.data[i][j]= this->data[i][j]/x;
    return ret;
}
matrix matrix::operator /=(double x)
{
    return *this= *this/x;
}
ostream& operator << (ostream& out, matrix x)
{
    for(uint8_t i=0; i<x.lines; i++)
    {
        for(uint8_t j=0; j<x.columns; j++)
        {
            out << x.data[i][j] << " ";
        }
        out << endl;
    }
    return out;
}

matrix operator -(matrix x)
{
    for(uint8_t i=0; i<x.lines; i++)
        for(uint8_t j=0; j<x.columns; j++)
            x.data[i][j]= -x.data[i][j];
    return x;
}

matrix matrix::identity(uint8_t n)
{
    matrix ret(n);
    for(uint8_t i=0; i<n; i++)
        for(uint8_t j=0; j<n; j++)
            if(i == j)
                ret.data[i][i]= 1;
            else
                ret.data[i][j]= 0;
    return ret;
}
matrix matrix::transposta(matrix x)
{
    matrix ret(x.columns, x.lines);
    for(uint8_t i=0; i<x.lines; i++)
        for(uint8_t j=0; j<x.columns; j++)
            ret.data[j][i]= x.data[i][j];
    return ret;
}