#include "PetriNetUsing.h"

#include <random>

double getRandom()
{
    static std::default_random_engine re;
    static std::uniform_real_distribution<double> rd;
    return rd(re);
}