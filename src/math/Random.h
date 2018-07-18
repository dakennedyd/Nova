#pragma once
#include <cstdint>
#include <random>

namespace Nova
{
class Random
{
  protected:
    std::mt19937_64 rndEngine{std::random_device()()};
    std::uniform_int_distribution<> intDistribution;
    std::uniform_real_distribution<> realDistribution;

  public:
    Random()
    {
        uint64_t seed = generateSeed();
        setSeed(seed);
    };
    Random(uint64_t seed) { setSeed(seed); };
    ~Random(){};

    uint64_t generateSeed() { return std::random_device()(); };
    void setSeed(uint64_t seed) { rndEngine.seed(seed); };
    int nextInteger() { return intDistribution(rndEngine); };
    int nextInteger(int min, int max);
    double nextDouble() { return realDistribution(rndEngine); };
    float nextFloat() { return static_cast<float>(realDistribution(rndEngine)); };
    double nextDouble(double min, double max);
    float nextFloat(float min, float max);
};
} // namespace Nova
