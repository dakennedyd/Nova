
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
#include "graphics/ILight.h"
#include "math/Vector.h"
#include <cstdint>

namespace Nova
{
class Light : public ILight
{
  public:
    Light() = delete;
    Light(int type, Vec3 *pos, Vec3 *color);  //, Vec3 *amb, Vec3 *diff, Vec3 *spec);
    ~Light() = default;                       //{ mIDCounter--; };
    Light(const Light &) = delete;            // copy ctor
    Light(Light &&other);                     // move ctor
    Light &operator=(Light const &) = delete; // copy assignment op
    Light &operator=(Light &&other);          // move assignment op

    const Vec3 *getPosition() const { return mPosition; };
    const Vec3 *getColor() const { return mColor; };
    /*const Vec3 *getAmbientColor() const;
    const Vec3 *getDiffuseColor() const;
    const Vec3 *getSpecularColor() const;*/
    const int getTypeCode() const { return mType; };

    const std::uint64_t getID() const { return mID; };
    /*float mConstant = 1.0f;
    float mLinear = 0.7f;
    float mQuadratic = 1.1f;*/

  private:
    /*Vec3 *mPosition;
    Vec3 *mAmbient;
    Vec3 *mDiffuse;
    Vec3 *mSpecular;*/

    Vec3 *mPosition;
    Vec3 *mColor;
    int mType;
    std::uint64_t mID;
    // static std::uint64_t mIDCounter;
};
} // namespace Nova
