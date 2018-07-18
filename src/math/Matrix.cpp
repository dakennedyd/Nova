#include "Matrix.h"
#include "logger/Logger.h"
#include "math/Vector.h"
#include <array>
#include <cmath>
#include <stdexcept>

namespace Nova
{
Mat4::Mat4() : mData{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} {}

Mat4::~Mat4() {}

Mat4::Mat4(const float value)
    : mData{value, 0, 0, 0, 0, value, 0, 0, 0, 0, value, 0, 0, 0, 0, value}
{
}

Mat4::Mat4(const std::array<float, 16> &values)
{
    // note: check for correct size of values array
    for (size_t i = 0; i < values.size(); ++i)
    {
        mData[i] = values[i];
    }
}

Mat4::Mat4(const Mat3 &mat)
{
    const float *data = mat.getDataPtr();
    mData[0] = data[0];
    mData[1] = data[1];
    mData[2] = data[2];
    mData[3] = 0;
    mData[4] = data[3];
    mData[5] = data[4];
    mData[6] = data[5];
    mData[7] = 0;
    mData[8] = data[6];
    mData[9] = data[7];
    mData[10] = data[8];
    mData[11] = 0;
    mData[12] = 0;
    mData[13] = 0;
    mData[14] = 0;
    mData[15] = 1; // should this be 1?
}

Mat4 Mat4::operator*(const Mat4 &right) const
{
    Mat4 result;
    float sum = 0;
    for (size_t i = 0; i < 4; ++i)
    {
        for (size_t j = 0; j < 4; ++j)
        {
            for (size_t k = 0; k < 4; ++k)
            {
                sum += mData[i * 4 + k] * right.mData[k * 4 + j];
            }
            result.mData[i * 4 + j] = sum;
            sum = 0;
        }
    }
    return result;
}

Vec4 Mat4::operator*(const Vec4 &right) const
{
    Vec4 result;

    result.setX(mData[0] * right.mData[0] + mData[1] * right.mData[1] + mData[2] * right.mData[2] +
                mData[3] * right.mData[3]);
    result.setY(mData[4] * right.mData[0] + mData[5] * right.mData[1] + mData[6] * right.mData[2] +
                mData[7] * right.mData[3]);
    result.setZ(mData[8] * right.mData[0] + mData[9] * right.mData[1] + mData[10] * right.mData[2] +
                mData[11] * right.mData[3]);
    result.setW(mData[12] * right.mData[0] + mData[13] * right.mData[1] +
                mData[14] * right.mData[2] + mData[15] * right.mData[3]);

    return result;
}

Mat4 Mat4::transpose() const
{
    return Mat4({mData[0], mData[4], mData[8], mData[12], mData[1], mData[5], mData[9], mData[13],
                 mData[2], mData[6], mData[10], mData[14], mData[3], mData[7], mData[11],
                 mData[15]});
}

Mat4 Mat4::calcNormalMatrix() const
{
    // taken from
    // http://stackoverflow.com/questions/27600045/the-correct-way-to-calculate-normal-matrix

    Vec3 r0(mData[0], mData[1], mData[2]);
    Vec3 r1(mData[4], mData[5], mData[6]);
    Vec3 r2(mData[8], mData[9], mData[10]);

    const Vec3 &row0 = r1 ^ r2;
    const Vec3 &row1 = r2 ^ r0;
    const Vec3 &row2 = r0 ^ r1;

    return Mat4({row0.getX(), row0.getY(), row0.getZ(), 0.0f, row1.getX(), row1.getY(), row1.getZ(),
                 0.0f, row2.getX(), row2.getY(), row2.getZ(), 0.0f, 0.0f, 0.0f, 0.0f, 0.0f});
}

const float *Mat4::getDataPtr() const { return mData; }

void Mat4::debugPrint() const
{
    LOG_DEBUG("Mat4:");
    for (int i = 0; i < 4; ++i)
    {
        LOG_DEBUG(mData[0 + i * 4] << " " << mData[1 + i * 4] << " " << mData[2 + i * 4] << " "
                                   << mData[3 + i * 4]);
    }
}

/* vertFOV takes angles in radians*/
Mat4 Mat4::makePerspectiveMatrix(const float vertFOV, const float aspectRatio,
                                 const float nearPlane, const float farPlane)
{
    // row major
    float yScale = 1.0f / tanf(vertFOV * 0.5f);
    float xScale = yScale / aspectRatio;
    float nearmfar = nearPlane - farPlane;
    return Mat4({xScale, 0.0f, 0.0f, 0.0f, 0.0f, yScale, 0.0f, 0.0f, 0.0f, 0.0f,
                 (farPlane + nearPlane) / nearmfar, -1.0f, 0.0f, 0.0f,
                 (2.0f * farPlane * nearPlane) / nearmfar, 0.0f});

    // column major
    /*float yScale = 1.0f / tanf(vertFOV / 2.0f);
    float xScale = yScale / aspectRatio;
    float nearmfar = nearPlane - farPlane;
    float m[16] = {
            xScale,	0.0f,		0.0f,
    0.0f, 0.0f,	yScale,		0.0f,
    0.0f, 0.0f,	0.0f,		(farPlane + nearPlane) / nearmfar,		(2.0f *
    farPlane*nearPlane) / nearmfar, 0.0f,	0.0f,		-1.0f,
    0.0f
    };
    return Mat4(m);*/
}

Mat4 Mat4::makeOrthographicMatrix(const float width, const float height, const float nearPlane,
                                  const float farPlane)
{
    return Mat4({2.0f / width, 0.0f, 0.0f, 0.0f, 0.0f, 2.0f / height, 0.0f, 0.0f, 0.0f, 0.0f,
                 -2.0f / (farPlane - nearPlane), -1.0f, 0.0f, 0.0f,
                 -(farPlane + nearPlane) / (farPlane - nearPlane), 0.0f});
}

Mat4 Mat4::makeOrthographicMatrix(const float right, const float left, const float top,
                                  const float bottom, const float nearPlane, const float farPlane)
{
    float rpl = right + left;
    float tpb = top + bottom;
    float rml = right - left;
    float tmb = top - bottom;
    return Mat4({2.0f / rml, 0.0f, 0.0f, -(rpl / rml), 0.0f, 2.0f / tmb, 0.0f, -(tpb / tmb), 0.0f,
                 0.0f, -2.0f / (farPlane - nearPlane),
                 -(farPlane + nearPlane) / (farPlane - nearPlane), 0.0f, 0.0f, 0.0f, 1.0f});
}

/**
 * Creates a rotation matrix
 * Matrix by Ronald Goldman
 * Extracted from Real Time Rendering 3rd Ed. Page 71
 */
Mat4 Mat4::makeRotationMatrix(const Vec3 &rotVector, const float angle)
{
    Vec3 nRotVector = rotVector;
    nRotVector.normalizeSelf();
    const float cosTheta = cosf(angle);
    const float sinTheta = sinf(angle);
    const float t = 1.0f - cosTheta;
    const float rx = nRotVector.getX();
    const float ry = nRotVector.getY();
    const float rz = nRotVector.getZ();
    Mat4 result;

    // right handed
    result.mData[0] = t * rx * rx + cosTheta;
    result.mData[1] = t * rx * ry + sinTheta * rz;
    result.mData[2] = t * rx * rz - sinTheta * ry;

    result.mData[4] = t * rx * ry - sinTheta * rz;
    result.mData[5] = t * ry * ry + cosTheta;
    result.mData[6] = t * ry * rz + sinTheta * rx;

    result.mData[8] = t * rx * rz + sinTheta * ry;
    result.mData[9] = t * ry * rz - sinTheta * rx;
    result.mData[10] = t * rz * rz + cosTheta;

    result.mData[15] = 1.0f;

    // left handed
    /*result.mData[0] = t*rx*rx + cosTheta;
    result.mData[1] = t*rx*ry - sinTheta*rz;
    result.mData[2] = t*rx*rz + sinTheta*ry;

    result.mData[4] = t*rx*ry + sinTheta*rz;
    result.mData[5] = t*ry*ry + cosTheta;
    result.mData[6] = t*ry*rz - sinTheta*rx;

    result.mData[8] = t*rx*rz - sinTheta*ry;
    result.mData[9] = t*ry*rz + sinTheta*rx;
    result.mData[10] = t*rz*rz + cosTheta;

    result.mData[15] = 1.0f;*/

    return result;
}

Mat4 Mat4::makeTranslationMatrix(const Vec3 &transVector)
{
    Mat4 result(1);
    result.mData[12] = transVector.getX();
    result.mData[13] = transVector.getY();
    result.mData[14] = transVector.getZ();
    return result;
}

Mat4 Mat4::makeScalingMatrix(const Vec3 &scaleVector)
{
    Mat4 result;
    result.mData[0] = scaleVector.getX();
    result.mData[5] = scaleVector.getY();
    result.mData[10] = scaleVector.getZ();
    result.mData[15] = 1.0f;

    return result;
}
Mat4 Mat4::makeScalingMatrix(const float scaleVector)
{
    Mat4 result;
    result.mData[0] = scaleVector;
    result.mData[5] = scaleVector;
    result.mData[10] = scaleVector;
    result.mData[15] = 1.0f;

    return result;
}
Mat4 Mat4::makeIdentityMatrix()
{
    Mat4 result(1);
    return result;
}

Mat4 Mat4::makeLookAtMatrix(const Vec3 &positionVector, const Vec3 &targetVector,
                            const Vec3 &upVector)
{
    Mat4 result;
    Vec3 forwardVector, leftVector, newUpVector;
    forwardVector = targetVector - positionVector;
    forwardVector.normalizeSelf();
    leftVector = forwardVector ^ upVector;
    // leftVector = upVector ^ forwardVector;
    leftVector.normalizeSelf();
    newUpVector = leftVector ^ forwardVector;
    // newUpVector = forwardVector ^ leftVector;
    // newUpVector.normalizeSelf();

    result.mData[0] = leftVector.getX();
    result.mData[4] = leftVector.getY();
    result.mData[8] = leftVector.getZ();

    result.mData[1] = newUpVector.getX();
    result.mData[5] = newUpVector.getY();
    result.mData[9] = newUpVector.getZ();

    result.mData[2] = forwardVector.getX();
    result.mData[6] = forwardVector.getY();
    result.mData[10] = forwardVector.getZ();

    result.mData[12] = (positionVector * leftVector);
    result.mData[13] = (positionVector * newUpVector);
    result.mData[14] = (positionVector * forwardVector);

    result.mData[15] = 1.0f;
    return result;
}

Mat4 Mat4::removeTranslation() const
{
    return Mat4({mData[0], mData[1], mData[2], mData[3], mData[4], mData[5], mData[6], mData[7],
                 mData[8], mData[9], mData[10], mData[11], 0.0f, 0.0f, 0.0f, mData[15]});
}

Vec3 Mat4::getTranslation() const { return Vec3(mData[12], mData[13], mData[14]); }

Mat3 Mat4::toMat3() const
{
    return Mat3({mData[0], mData[1], mData[2], mData[4], mData[5], mData[6], mData[8], mData[9],
                 mData[10]});
}

/*======================*/
/*======  Mat3 =========*/
/*======================*/

Mat3::Mat3() : mData{0, 0, 0, 0, 0, 0, 0, 0, 0} {}

Mat3::~Mat3() {}
Mat3::Mat3(const std::array<float, 9> &values)
{
    // note: check for correct size of values array
    for (size_t i = 0; i < values.size(); ++i)
    {
        mData[i] = values[i];
    }
}
// Mat3::Mat3(const Mat4 & mat)
// {
// 	const float* data = mat.getDataPtr();
// 	mData[0] = data[0]; mData[1] = data[1]; mData[2] = data[2];
// 	mData[3] = data[4]; mData[4] = data[5]; mData[5] = data[6];
// 	mData[6] = data[8]; mData[7] = data[9]; mData[8] = data[10];
// }
Vec3 Mat3::operator*(const Vec3 &right) const
{
    Vec3 result;

    result.setX(mData[0] * right.getX() + mData[1] * right.getY() + mData[2] * right.getZ());
    result.setY(mData[3] * right.getX() + mData[4] * right.getY() + mData[5] * right.getZ());
    result.setZ(mData[6] * right.getX() + mData[7] * right.getY() + mData[8] * right.getZ());

    return result;
}
Mat3 Mat3::operator/(const float value) const
{
    return Mat3({mData[0] / value, mData[1] / value, mData[2] / value, mData[3] / value,
                 mData[4] / value, mData[5] / value, mData[6] / value, mData[7] / value,
                 mData[8] / value});
}
float Mat3::determinant() const
{
    float result = mData[0] * (mData[4] * mData[8] - mData[5] * mData[7]) +
                   mData[1] * (mData[5] * mData[6] - mData[3] * mData[8]) +
                   mData[2] * (mData[3] * mData[7] - mData[4] * mData[6]);

    return result;
}
Mat3 Mat3::inverse() const
{
    const Mat3 &adjoint = makeCofactorsMatrix().transpose();
    return adjoint / determinant();
}
Mat3 Mat3::transpose() const
{
    return Mat3(
        {mData[0], mData[3], mData[6], mData[1], mData[4], mData[7], mData[2], mData[5], mData[8]});
}

const float *Mat3::getDataPtr() const { return mData; }
Mat3 Mat3::makeRotationMatrix(const Vec3 &rotVector, const float angle)
{
    Vec3 rotAxis = rotVector;
    rotAxis.normalizeSelf();
    const float cosTheta = cosf(angle);
    const float sinTheta = sinf(angle);
    const float rx = rotVector.getX();
    const float ry = rotVector.getY();
    const float rz = rotVector.getZ();
    Mat3 result;

    result.mData[0] = cosTheta + (rx * rx) * (1 - cosTheta);
    result.mData[1] = (1 - cosTheta) * rx * ry - rz * sinTheta;
    result.mData[2] = (1 - cosTheta) * rx * rz + ry * sinTheta;
    result.mData[3] = (1 - cosTheta) * rx * ry + rz * sinTheta;
    result.mData[4] = cosTheta + ry * ry * (1 - cosTheta);
    result.mData[5] = (1 - cosTheta) * ry * rz - rx * sinTheta;
    result.mData[6] = (1 - cosTheta) * rx * rz - ry * sinTheta;
    result.mData[7] = (1 - cosTheta) * ry * rz + rx * sinTheta;
    result.mData[8] = cosTheta + rz * rz * (1 - cosTheta);

    return result;
}
Mat3 Mat3::makeCofactorsMatrix() const
{
    return Mat3({mData[4] * mData[8] - mData[5] * mData[7],
                 (mData[3] * mData[8] - mData[5] * mData[6]) * -1.0f,
                 mData[3] * mData[7] - mData[4] * mData[6],
                 (mData[1] * mData[8] - mData[2] * mData[7]) * -1.0f,
                 mData[0] * mData[8] - mData[2] * mData[6],
                 (mData[0] * mData[7] - mData[1] * mData[6]) * -1.0f,
                 mData[1] * mData[5] - mData[2] * mData[4],
                 (mData[0] * mData[5] - mData[2] * mData[3]) * -1.0f,
                 mData[0] * mData[4] - mData[1] * mData[3]});
}
} // namespace Nova