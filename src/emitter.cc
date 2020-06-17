#include "emitter.hh"


float random_range(float up)
{
    return (float)(rand()) / (float)(RAND_MAX/up);
}

float random_range(float min, float up)
{
    return random_range(up - min) + min;
}
