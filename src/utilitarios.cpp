#include "utilitarios.h"

template<class T>
T abs(T n)
{
    return (n>0) ? (n) : (-n);
}
bool dEqual(double n1, double n2, double range)
{
    return abs(n1-n2)<range;
}
