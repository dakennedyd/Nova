
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

#include "Vector.h"
#include "logger/Logger.h"
#include "math/Quaternion.h"
#include <cmath>

namespace Nova
{
class UnitQuat;

Vec3::Vec3() : mData{0.0f, 0.0f, 0.0f} {}

Vec3::Vec3(const float value) : mData{value, value, value} {}

Vec3::Vec3(const float x, const float y, const float z) : mData{x, y, z} {}

Vec3 Vec3::operator*(const float value) const { return value * (*this); }

Vec3 Vec3::operator/(const float value) const { return value / (*this); }

const float *Vec3::getDataPtr() const { return mData; }

void Vec3::rotateSelf(const Vec3 &rotAxis, const float angle)
{
    Vec3 thisVector(*this);
    // Vec3 nRotAxis(rotAxis);
    // nRotAxis.normalizeSelf();
    *this = thisVector * cos(angle) + (rotAxis ^ thisVector) * sin(angle) +
            rotAxis * (rotAxis * thisVector) * (1.0f - cos(angle));
}

void Vec3::rotateSelf(const UnitQuat &q)
{
    UnitQuat p(mData[0], mData[1], mData[2], 1.0f);
    p = q * p * q.conjugate();

    mData[0] = p.mX;
    mData[1] = p.mY;
    mData[2] = p.mZ;
}

Vec3 Vec3::rotate(const UnitQuat &q) const
{
    UnitQuat p(mData[0], mData[1], mData[2], 1.0f);
    p = q * p * q.conjugate();

    return Vec3(p.mX, p.mY, p.mZ);
}

Vec3 Vec3::rotate(const Vec3 &rotAxis, const float angle) const
{
    Vec3 returnValue(mData[0], mData[1], mData[2]);
    returnValue.rotateSelf(rotAxis, angle);
    return returnValue;
}

float Vec3::getX() const { return mData[0]; }

float Vec3::getY() const { return mData[1]; }

float Vec3::getZ() const { return mData[2]; }

void Vec3::setX(const float value)
{
    mMagnitud = -1.0f;
    mData[0] = value;
}

void Vec3::setY(const float value)
{
    mMagnitud = -1.0f;
    mData[1] = value;
}

void Vec3::setZ(const float value)
{
    mMagnitud = -1.0f;
    mData[2] = value;
}

float Vec3::getMagnitud()
{
    if (mMagnitud == -1.0f)
    {
        mMagnitud = std::sqrt(mData[0] * mData[0] + mData[1] * mData[1] + mData[2] * mData[2]);
    }
    return mMagnitud;
}

float Vec3::getDistance(const Vec3 &v) { return (*this - v).getMagnitud(); }

void Vec3::normalizeSelf()
{
    getMagnitud();
    if (mMagnitud != 1.0f)
    {
        mData[0] /= mMagnitud;
        mData[1] /= mMagnitud;
        mData[2] /= mMagnitud;
        mMagnitud = 1.0f;
    }
    return;
}

Vec3 Vec3::normalize() const
{
    Vec3 r = *this;

    r.mData[0] /= mMagnitud;
    r.mData[1] /= mMagnitud;
    r.mData[2] /= mMagnitud;
    r.mMagnitud = 1.0f;

    return r;
}

bool Vec3::isNormalized() const
{
    if (mMagnitud == 1.0f)
    {
        return true;
    }
    return false;
}

void Vec3::debugPrint() const
{
    LOG_DEBUG("Vector:" << mData[0] << " " << mData[1] << " " << mData[2]);
}

/*======================*/
/*======  Vec4 =========*/
/*======================*/

Vec4::Vec4() : mData{0.0f, 0.0f, 0.0f, 0.0f} {}

Vec4::Vec4(const float value) : mData{value, value, value} {}

Vec4::Vec4(const float x, const float y, const float z, const float w) : mData{x, y, z, w} {}

Vec4::Vec4(const Vec3 &vec, const float w) : mData{vec.getX(), vec.getY(), vec.getZ(), w} {}

/**
 * Scalar Product
 */

const float *Vec4::getDataPtr() const { return mData; }

float Vec4::getX() const { return mData[0]; }

float Vec4::getY() const { return mData[1]; }

float Vec4::getZ() const { return mData[2]; }

float Vec4::getW() const { return mData[3]; }

void Vec4::setX(const float value)
{
    mMagnitud = -1.0f;
    mData[0] = value;
}

void Vec4::setY(const float value)
{
    mMagnitud = -1.0f;
    mData[1] = value;
}

void Vec4::setZ(const float value)
{
    mMagnitud = -1.0f;
    mData[2] = value;
}

void Vec4::setW(const float value)
{
    mMagnitud = -1.0f;
    mData[3] = value;
}

Vec4 Vec4::rotate(const UnitQuat &q) const
{
    UnitQuat p(mData[0], mData[1], mData[2], mData[3]);
    p = q * p * q.conjugate();

    return Vec4(p.mX, p.mY, p.mZ, p.mW);
}

float Vec4::getMagnitud()
{
    if (mMagnitud == -1.0f)
    {
        mMagnitud = sqrtf(mData[0] * mData[0] + mData[1] * mData[1] + mData[2] * mData[2] +
                          mData[3] * mData[3]);
    }
    return mMagnitud;
}

void Vec4::normalizeSelf()
{
    getMagnitud();
    if (mMagnitud != 1.0f)
    {
        mData[0] /= mMagnitud;
        mData[1] /= mMagnitud;
        mData[2] /= mMagnitud;
        mData[3] /= mMagnitud;
        mMagnitud = 1.0f;
    }
    return;
}

void Vec4::debugPrint() const
{
    LOG_DEBUG("Vector:" << mData[0] << " " << mData[1] << " " << mData[2] << " " << mData[3]);
}

Vec3 operator+(const Vec3 &left, const Vec3 &right)
{
    Vec3 result;
    /* used setter method because vector class
    has lazy evaluation of magnitud*/
    result.setX(left.mData[0] + right.mData[0]);
    result.setY(left.mData[1] + right.mData[1]);
    result.setZ(left.mData[2] + right.mData[2]);

    return result;
}

Vec3 operator-(const Vec3 &left, const Vec3 &right)
{
    Vec3 result;
    /* used setter method because vector class
    has lazy evaluation of magnitud*/
    result.setX(left.mData[0] - right.mData[0]);
    result.setY(left.mData[1] - right.mData[1]);
    result.setZ(left.mData[2] - right.mData[2]);

    return result;
}

float operator*(const Vec3 &left, const Vec3 &right)
{
    return left.mData[0] * right.mData[0] + left.mData[1] * right.mData[1] +
           left.mData[2] * right.mData[2];
}
Vec3 operator*(const float value, const Vec3 &left)
{
    return Vec3(left.mData[0] * value, left.mData[1] * value, left.mData[2] * value);
}
Vec3 operator/(const float value, const Vec3 &left)
{
    return Vec3(left.mData[0] / value, left.mData[1] / value, left.mData[2] / value);
}
Vec3 operator^(const Vec3 &left, const Vec3 &right)
{
    Vec3 result;
    /* used setter method because vector class
    has lazy evaluation of magnitud*/
    result.setX((left.mData[1]) * right.mData[2] - (left.mData[2]) * right.mData[1]);
    result.setY((left.mData[2]) * right.mData[0] - (left.mData[0]) * right.mData[2]);
    result.setZ((left.mData[0]) * right.mData[1] - (left.mData[1]) * right.mData[0]);

    return result;
}
Vec4 operator+(const Vec4 &left, const Vec4 &right)
{
    Vec4 result;
    /* used setter method because vector class
    has lazy evaluation of magnitud*/
    result.setX(left.mData[0] + right.mData[0]);
    result.setY(left.mData[1] + right.mData[1]);
    result.setZ(left.mData[2] + right.mData[2]);
    result.setW(left.mData[3] + right.mData[3]);

    return result;
}
Vec4 operator-(const Vec4 &left, const Vec4 &right)
{
    Vec4 result;
    /* used setter method because vector class
    has lazy evaluation of magnitud*/
    result.setX(left.mData[0] - right.mData[0]);
    result.setY(left.mData[1] - right.mData[1]);
    result.setZ(left.mData[2] - right.mData[2]);
    result.setW(left.mData[3] - right.mData[3]);

    return result;
}
float operator*(const Vec4 &left, const Vec4 &right)
{
    return left.mData[0] * right.mData[0] + left.mData[1] * right.mData[1] +
           left.mData[2] * right.mData[2] + left.mData[3] * right.mData[3];
}
} // namespace Nova