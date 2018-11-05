
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

#include "Quaternion.h"
#include "logger/Logger.h"
#include "math/Matrix.h"
#include "math/Vector.h"
#include <cmath>

namespace Nova
{
UnitQuat::UnitQuat() : UnitQuat(0.0f, 0.0f, 0.0f, 1.0f) {}
UnitQuat::UnitQuat(const float x, const float y, const float z, const float w)
    : mX(x), mY(y), mZ(z), mW(w)
{
}

UnitQuat::UnitQuat(const Vec3 &axis, const float angle)
{
    // NOVA_ASSERT(axis.isNormalized())
    float halfAngle = angle * 0.5f;
    float sinHalfAangle = sin(halfAngle);

    mX = axis.mData[0] * sinHalfAangle;
    mY = axis.mData[1] * sinHalfAangle;
    mZ = axis.mData[2] * sinHalfAangle;
    mW = cos(halfAngle);
}

UnitQuat::UnitQuat(const Vec4 &vec) : mX(vec.getX()), mY(vec.getY()), mZ(vec.getZ()), mW(vec.getW())
{
}

UnitQuat::UnitQuat(const float pitch, const float yaw, const float roll)
{
    float a = cos(yaw * 0.5f);
    float b = cos(pitch * 0.5f);
    float c = cos(roll * 0.5f);
    float d = sin(yaw * 0.5f);
    float e = sin(pitch * 0.5f);
    float f = sin(roll * 0.5f);

    /*mW = a*b*c + d*e*f;

    mX = a*f*b - d*c*e;
    mY = a*c*e + d*f*b;
    mZ = d*c*b - a*f*e;*/

    mW = a * b * c + d * e * f;

    mX = a * e * c + d * b * f;
    mY = d * b * c - a * e * f;
    mZ = a * b * f - d * e * c;

    /*mW = a*b*c + d*e*f;

    mX = -a*e*c - d*b*f;
    mY = a*e*f - d*b*c;
    mZ = d*e*c - a*b*f;*/
}

UnitQuat UnitQuat::operator*(const UnitQuat &right) const
{
    // pq = [ (pS q V + q S p V + p V × q V) (p S q S − p V · q V) ]
    Vec3 pv(mX, mY, mZ);
    Vec3 qv(right.getVector());
    float ps = mW;
    float qs = right.getScalar();

    Vec3 v = (ps * qv) + (qs * pv) + (pv ^ qv);
    float s = ps * qs - pv * qv;
    return UnitQuat(v.mData[0], v.mData[1], v.mData[2], s);
    // return UnitQuat(mY*right.mZ - mZ*right.mY + mW*right.mX + mW*right.mX,
    //                         mZ*right.mX - mX*right.mZ + mW*right.mY + mW*right.mX,
    //                         mX*right.mY - mY*right.mX + mW*right.mZ + mW*right.mZ,
    //                         mW*right.mW - mX*right.mX - mY*right.mY - mZ*right.mZ);
}

Vec3 UnitQuat::getVector() const { return Vec3(mX, mY, mZ); }

float UnitQuat::getScalar() const { return mW; }

void UnitQuat::debugPrint()
{
    LOG_DEBUG("Quaternion:" << mX << " " << mY << " " << mZ << " " << mW);
}

void UnitQuat::normalizeSelf()
{
    float invNorm = 1.0f / sqrtf(mX * mX + mY * mY + mZ * mZ + mW * mW);
    mX *= invNorm;
    mY *= invNorm;
    mZ *= invNorm;
    mW *= invNorm;
}

Mat4 UnitQuat::toRotationMatrix4() const
{
    // taken from "3D Math Primer for Graphics and Game Development" 2nd Edition page 284
    return Mat4(std::array<float, 16>{
        1.0f - 2.0f * mY * mY - 2.0f * mZ * mZ, 2.0f * mX * mY + 2.0f * mW * mZ,
        2.0f * mX * mZ - 2.0f * mW * mY, 0.0f, 2.0f * mX * mY - 2.0f * mW * mZ,
        1.0f - 2.0f * mX * mX - 2.0f * mZ * mZ, 2.0f * mY * mZ + 2.0f * mW * mX, 0.0f,
        2.0f * mX * mZ + 2.0f * mW * mY, 2.0f * mY * mZ - 2.0f * mW * mX,
        1.0f - 2.0f * mX * mX - 2.0f * mY * mY, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f});
}

Mat3 UnitQuat::toRotationMatrix3() const
{
    // taken from "3D Math Primer for Graphics and Game Development" 2nd Edition page 284
    return Mat3(std::array<float, 9>{
        1.0f - 2.0f * mY * mY - 2.0f * mZ * mZ, 2.0f * mX * mY + 2.0f * mW * mZ,
        2.0f * mX * mZ - 2.0f * mW * mY, 2.0f * mX * mY - 2.0f * mW * mZ,
        1.0f - 2.0f * mX * mX - 2.0f * mZ * mZ, 2.0f * mY * mZ + 2.0f * mW * mX,
        2.0f * mX * mZ + 2.0f * mW * mY, 2.0f * mY * mZ - 2.0f * mW * mX,
        1.0f - 2.0f * mX * mX - 2.0f * mY * mY});
}

float UnitQuat::getX() const { return mX; }

float UnitQuat::getY() const { return mY; }

float UnitQuat::getZ() const { return mZ; }

float UnitQuat::getW() const { return mW; }
} // namespace Nova