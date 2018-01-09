#ifndef RANDOM_H
#define RANDOM_H
#include <random>
class Random
{
public:
    Random();
    void seed();
    void seed(std::mt19937::result_type seed);
    std::mt19937::result_type rResult();
    int rInt();
    float rFloat();
    float range(float a, float b);
private:
    std::random_device device;
    std::mt19937 engine;
    std::uniform_int_distribution<> intDistribution;
    std::uniform_real_distribution<float> realDistribution;
};
#endif // RANDOM_H
