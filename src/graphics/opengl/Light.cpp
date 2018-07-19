
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

#include "Light.h"
#include "math/Vector.h"
#include <utility>

namespace Nova
{
// std::uint64_t Light::mIDCounter = 0;
Light::Light(int type, Vec3 *pos, Vec3 *color) //, Vec3 * amb, Vec3 * diff, Vec3 * spec)
    : mPosition(pos), mColor(color), mType(type)
{
    // mID = mIDCounter;
    // mIDCounter++;
}
Light::Light(Light &&other)
    : mPosition(std::move(other.mPosition)), mColor(std::move(other.mColor)),
      mType(std::move(other.mType))
/*mAmbient(std::move(other.mAmbient)),
mDiffuse(std::move(other.mDiffuse)), mSpecular(std::move(other.mSpecular)),
mConstant(std::move(other.mConstant)), mLinear(std::move(other.mLinear)),
mQuadratic(std::move(other.mQuadratic))*/
// mID(std::move(other.mID))
{
}
Light &Light::operator=(Light &&other)
{
    if (&other != this)
    {
        mType = std::move(other.mType);
        mPosition = std::move(other.mPosition);
        mColor = std::move(other.mColor);
        /*mAmbient = std::move(other.mAmbient);
        mDiffuse = std::move(other.mDiffuse);
        mSpecular = std::move(other.mSpecular);
        mConstant = std::move(other.mConstant);
        mLinear = std::move(other.mLinear);
        mQuadratic = std::move(other.mQuadratic);*/
        // mID = std::move(other.mID);
    }
    return *this;
}
/*const Vec3 * Light::getAmbientColor() const
{
        return mAmbient;
}
const Vec3 * Light::getDiffuseColor() const
{
        return mDiffuse;
}
const Vec3 * Light::getSpecularColor() const
{
        return mSpecular;
}*/
} // namespace Nova