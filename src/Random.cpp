#include "Random.h"
#include <ctime>
Random::Random()
    :device()
    ,engine()
    ,intDistribution()
    ,realDistribution()
{
}
void Random::seed()
{
    if(device.entropy())
    {
        engine.seed(device());
    }
    else
    {
        engine.seed(time(0));
    }
}
void Random::seed(std::mt19937::result_type seed)
{
    engine.seed(seed);
}
std::mt19937::result_type Random::rResult()
{
    return engine();
}
int Random::rInt()
{
    return intDistribution(engine);
}
float Random::rFloat()
{
    return realDistribution(engine);
}
float Random::range(float a, float b)
{
    float diff = b - a;
    return a + rFloat()*diff;
}
