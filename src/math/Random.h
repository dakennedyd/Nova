
// The MIT License (MIT)

// Copyright (c) 2018 David Kennedy

//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.

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
