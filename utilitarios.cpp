#include "utilitarios.h"

bool dEqual(vec3 v1, vec3 v2)
{
    return dEqual(v1[VX], v2[VX]) && dEqual(v1[VY], v2[VY]) && dEqual(v1[VZ], v2[VZ]);
}
bool dEqual(double n1, double n2, double range)
{
    return abs(n1-n2)<range;
}
