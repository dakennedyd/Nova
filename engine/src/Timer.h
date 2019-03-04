
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
//#include <cstddef>
//#include <cstdint>
//#include <sys/types.h>
//#include <cstdlib>
//#include <chrono>
#include "Init.h"

namespace Nova
{
class Timer
{
  public:
    Timer() { reset(); };
    ~Timer() = default;
    inline void reset() { mEpoch = std::chrono::high_resolution_clock::now(); }
    inline long getMicro()
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> now =
            std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(now - mEpoch).count();
    }
    inline long getMillis()
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> now =
            std::chrono::high_resolution_clock::now();
        // std::chrono::duration<int, std::milli> r = now - mEpoch;
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - mEpoch).count();
    }
    inline long getSecs()
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> now =
            std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::seconds>(now - mEpoch).count();
    }

    /*gets the time since the engine started in milliseconds*/
    static long getTimeSinceEngineStart()
    {
        std::chrono::time_point<std::chrono::high_resolution_clock> now =
            std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - ENGINE_EPOCH).count();
    };

  private:
    std::chrono::time_point<std::chrono::high_resolution_clock> mEpoch;
};
} // namespace Nova
