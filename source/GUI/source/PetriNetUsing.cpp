#include "PetriNetUsing.h"

#include <random>

size_t getRandom(size_t upperBound)
{
    static std::default_random_engine re;
    std::uniform_real_distribution<double> rd(0, upperBound);
    size_t value = (size_t) rd(re);

    if (value == upperBound && value != 0)
    {
        --value;
    }

    return value;
}

double getRandom()
{
    static std::default_random_engine re;
    static std::uniform_real_distribution<double> rd(0, 1);
    return rd(re);
}