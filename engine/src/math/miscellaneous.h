
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
#include "Constants.h"
#include "PreInit.h"
#ifdef NOVA_WINDOWS_PLATFORM
#    include <cctype>
#endif
#include <algorithm>
#include <cmath>
#include <string>
#include <vector>

namespace Nova
{
static inline float toDegrees(const float value) { return value * (180.0f / NOVA_PI); }
static inline float toRadians(const float value) { return value * (NOVA_PI / 180.0f); }
static inline bool isEqual(float valueA, float valueB)
{
    // if ((abs(valueA) - abs(valueB)) <= NOVA_LAMBDA)
    if ((std::abs(valueA - valueB)) <= NOVA_LAMBDA)
    {
        return true;
    }
    return false;
}

/*takes an std::vector<string> and converts it to a std::vector<float>*/
static inline std::vector<float> vectorStringToFloat(const std::vector<std::string> &stringVector)
{
    std::vector<float> floatVector(stringVector.size());
    std::transform(stringVector.begin(), stringVector.end(), floatVector.begin(),
                   [](const std::string &val) { return stof(val); });
    return floatVector;
}

/** replace all ocurrences of string2 in string1 with string3 */
static inline void replaceAllInPlace(std::string &string1, const std::string &string2,
                                     const std::string &string3)
{
    size_t pos = 0;
    while ((pos = string1.find(string2, pos)) != std::string::npos)
    {
        string1.replace(pos, string2.length(), string3);
        pos += string3.length();
    }
}

/** parse tokens in a string and return them as a vector */
static inline std::vector<std::string> split(const std::string &str,
                                             const std::string &delimiter = " ")
{
    std::vector<std::string> result;
    std::string s(str);
    size_t pos = 0;
    std::string token;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        result.push_back(token);
        // LOG_DEBUG(token);
        s.erase(0, pos + delimiter.length());
    }
    result.push_back(s);
    // LOG_DEBUG(s);
    return result;
}

/**
trim string from start (in place)
*/
static inline void ltrim(std::string &s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) { return !std::isspace(ch); }));
}

/**
trim string from end (in place)
*/
static inline void rtrim(std::string &s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) { return !std::isspace(ch); }).base(),
            s.end());
}

/**
trim string from both ends (in place)
*/
static inline void trim(std::string &s)
{
    ltrim(s);
    rtrim(s);
}

/*template <bool>
struct if_constexpr;

template <>
struct if_constexpr<true>
{
        struct elser
        {
                template <class F, class... Args>
                void _else(F&&, Args&&...) {}

                template <bool C, class F, class... Args>
                auto _else_if(F&&, Args&&...) { return elser(); }
        };

        template <class F, class... Args>
        auto operator()(F&& callback, Args&&... args)
        {
                callback(std::forward<Args>(args)...);
                return elser();
        }
};

template <>
struct if_constexpr<false>
{
        struct elser
        {
                template <bool C, class F, class... Args>
                auto _else_if(F&& callback, Args&&... args)
                {
                        return if_constexpr<C>()(std::forward<F>(callback),
std::forward<Args>(args)...);
                }

                template <class F, class... Args>
                void _else(F&& callback, Args&&... args)
                {
                        callback(std::forward<Args>(args)...);
                }
        };

        template <class F, class... Args>
        auto operator()(F&& callback, Args&&... args)
        {
                return elser();
        }
};*/
} // namespace Nova