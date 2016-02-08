#ifndef UTILITARIOS_H
#define UTILITARIOS_H

#define M_PI 3.14159265358979323846

template<class T>
T abs(T n)
{
    return (n>0) ? (n) : (-n);
}
bool dEqual(double n1, double n2, double range= 1E-5);

#endif // UTILITARIOS_H
