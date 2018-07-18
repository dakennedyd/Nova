#pragma once
#include <array>

namespace Nova
{
class Vec3;
class Vec4;
class Mat3
{
    // friend class Vec3;
    // friend class Mat4;
  public:
    Mat3();
    ~Mat3();
    Mat3(const std::array<float, 9> &values);
    // Mat3(const Mat4& mat);
    Vec3 operator*(const Vec3 &right) const;
    Mat3 operator/(const float value) const;

    float determinant() const;
    Mat3 inverse() const;
    Mat3 transpose() const;
    const float *getDataPtr() const;
    static Mat3 makeRotationMatrix(const Vec3 &rotVector, const float angle);

  private:
    Mat3 makeCofactorsMatrix() const;
    float mData[9];
};

class Mat4
{
    // friend class Vec4;
  public:
    Mat4();
    ~Mat4();
    Mat4(const float value);
    Mat4(const std::array<float, 16> &values);
    Mat4(const Mat3 &mat);
    Mat4 operator*(const Mat4 &right) const;
    Vec4 operator*(const Vec4 &right) const;

    Mat4 transpose() const;
    Mat4 calcNormalMatrix() const;
    // friend Mat4 operator*(const Vec4& left, Mat4& right);
    const float *getDataPtr() const;

    void debugPrint() const;
    static Mat4 makePerspectiveMatrix(const float vertFOV, const float aspectRatio,
                                      const float nearPlane, const float farPlane);
    static Mat4 makeOrthographicMatrix(const float width, const float height, const float nearPlane,
                                       const float farPlane);
    static Mat4 makeOrthographicMatrix(const float right, const float left, const float top,
                                       const float bottom, const float nearPlane,
                                       const float farPlane);
    static Mat4 makeRotationMatrix(const Vec3 &rotVector, const float angle);
    static Mat4 makeTranslationMatrix(const Vec3 &transVector);
    static Mat4 makeScalingMatrix(const Vec3 &scaleVector);
    static Mat4 makeScalingMatrix(const float scaleVector);
    static Mat4 makeIdentityMatrix();
    static Mat4 makeLookAtMatrix(const Vec3 &positionVector, const Vec3 &targetVector,
                                 const Vec3 &upVector);

    Mat4 removeTranslation() const;
    Vec3 getTranslation() const;
    Mat3 toMat3() const;

  private:
    float mData[16];
};
} // namespace Nova