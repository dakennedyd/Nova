
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

namespace Nova
{
class Vec3;
class Vec4;
class Mat4;
class Mat3;
class UnitQuat
{
  public:
    friend class Vec3;
    friend class Vec4;
    friend class Mat3;
    friend class Mat4;
    UnitQuat();
    UnitQuat(const float x, const float y, const float z, const float w);

    /**
    constructs a rotation quaternion from an axis and an angle
    note: angle must be in radians, if axis is not an unit vector
    the result is undefined
    */
    UnitQuat(const Vec3 &axis, const float angle);
    UnitQuat(const Vec4 &vec);

    /**
     * creates a unit quaternion from Euler angles
     */
    UnitQuat(const float pitch, const float yaw, const float roll);
    ~UnitQuat() = default;

    UnitQuat operator*(const UnitQuat &right) const;
    // Vec3 operator*(const Vec3 & right) const;

    Vec3 getVector() const;
    float getScalar() const;
    void debugPrint();
    void normalizeSelf();
    UnitQuat conjugate() const { return UnitQuat(-mX, -mY, -mZ, mW); };
    Mat4 toRotationMatrix4() const;
    Mat3 toRotationMatrix3() const;

    float getX() const;
    float getY() const;
    float getZ() const;
    float getW() const;

    void setToIdentity()
    {
        mX = 0.0f;
        mY = 0.0f;
        mZ = 0.0f;
        mW = 1.0f;
    };

  private:
    float mX, mY, mZ, mW;
};
} // namespace Nova
