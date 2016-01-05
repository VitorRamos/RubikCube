#include "vec3.h"

vec3::vec3() : matrix(3,1)
{

}
vec3::vec3(float x, float y, float z) : matrix(3,1)
{
    data[0][0]= x;
    data[1][0]= y;
    data[2][0]= z;
}
vec3::vec3(matrix x) : matrix(3,1)
{
    *(this)= x;
}
vec3 vec3::operator =(matrix x)
{
    if(x.getLines() == 3 && x.getColumns() == 1)
    {
        this->data[0][0]= x.at(0,0);
        this->data[1][0]= x.at(1,0);
        this->data[2][0]= x.at(2,0);
        return *this;
    }
    else
        throw "Erro de assimilacao matrix vetor";
}
void vec3::set(float x, float y, float z)
{
    this->at(0)= x;
    this->at(1)= y;
    this->at(2)= z;
}
float vec3::x()
{
    return this->at(0);
}
float vec3::y()
{
    return this->at(1);
}
float vec3::z()
{
    return this->at(2);
}
float& vec3::at(int i)
{
    return this->matrix::at(i, 0);
}
float vec3::norma()
{
    return sqrt( pow(this->x(),2) + pow(this->y(),2) + pow(this->z(),2));
}
float vec3::produto(vec3 x)
{
    return this->x()*x.x()+this->y()*x.y()+this->z()*x.z();
}
vec3 vec3::cruzado(vec3 x)
{
    return vec3(this->y()*x.z()-this->z()*x.y(),
                this->z()*x.x()-this->x()*x.z(),
                this->x()*x.y()-this->y()*x.x());
}
vec3 vec3::projecao(vec3 x)
{
    return x*vec3::normalizar(*this);
}
vec3 vec3::normalizar(vec3 x)
{
    return (x.norma() == 0) ? (vec3(0,0,0)) :
        vec3(x.x()/x.norma(), x.y()/x.norma(), x.z()/x.norma());
}

ostream& operator << (ostream& out, vec3 x)
{
    out << matrix::transposta(x) << endl;
    return out;
}
