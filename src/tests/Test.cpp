
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

#define GLM_ENABLE_EXPERIMENTAL

#include "tests/Test.h"

#include "glm/ext.hpp"
#include "glm/glm.hpp"                  // vec3, vec4, ivec4, mat4
#include "glm/gtc/matrix_transform.hpp" // translate, rotateSelf, scale, perspective
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/type_ptr.hpp" // value_ptr
#include "glm/gtx/rotate_vector.hpp"
#include "logger/Logger.h"
#include "math/Matrix.h"
#include "math/Quaternion.h"
#include "math/Random.h"
#include "math/Vector.h"
#include "math/miscellaneous.h"
#include <limits>

using namespace Nova;

namespace Nova
{
namespace tests
{
static size_t errorCount = 0, errorCountTotal = 0;
void testVec3(const int iterations)
{
    Random random;
    errorCount = 0;
    LOG_DEBUG("****Vec3 testing started****");
    LOG_DEBUG("Add function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        glm::vec3 glmA(random.nextFloat(), random.nextFloat(), random.nextFloat());
        glm::vec3 glmB(random.nextFloat(), random.nextFloat(), random.nextFloat());
        Vec3 a(glmA.x, glmA.y, glmA.z);
        Vec3 b(glmB.x, glmB.y, glmB.z);
        Vec3 c = a + b;
        glm::vec3 glmC = glmA + glmB;
        if (!isEqual(c.getX(), glmC[0]))
        {
            error("Vec3 add function", c.getX(), glmC[0]);
            break;
        }
        if (!isEqual(c.getY(), glmC[1]))
        {
            error("Vec3 add function", c.getY(), glmC[1]);
            break;
        }
        if (!isEqual(c.getZ(), glmC[2]))
        {
            error("Vec3 add function", c.getZ(), glmC[2]);
            break;
        }
    }
    errorCount = 0;
    LOG_DEBUG("Subtract function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        glm::vec3 glmA(random.nextFloat(), random.nextFloat(), random.nextFloat());
        glm::vec3 glmB(random.nextFloat(), random.nextFloat(), random.nextFloat());
        Vec3 a(glmA.x, glmA.y, glmA.z);
        Vec3 b(glmB.x, glmB.y, glmB.z);
        Vec3 c = a - b;
        glm::vec3 glmC = glmA - glmB;
        if (!isEqual(c.getX(), glmC[0]))
        {
            error("Vec3 subtract function", c.getX(), glmC[0]);
            break;
        }
        if (!isEqual(c.getY(), glmC[1]))
        {
            error("Vec3 subtract function", c.getY(), glmC[1]);
            break;
        }
        if (!isEqual(c.getZ(), glmC[2]))
        {
            error("Vec3 subtract function", c.getZ(), glmC[2]);
            break;
        }
    }
    errorCount = 0;
    LOG_DEBUG("Dot product function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        glm::vec3 glmA(random.nextFloat(), random.nextFloat(), random.nextFloat());
        glm::vec3 glmB(random.nextFloat(), random.nextFloat(), random.nextFloat());
        Vec3 a(glmA.x, glmA.y, glmA.z);
        Vec3 b(glmB.x, glmB.y, glmB.z);
        if (!isEqual(a * b, glm::dot(glmA, glmB)))
        {
            error("Vec3 dot product function", a * b, glm::dot(glmA, glmB));
            break;
        }
    }
    errorCount = 0;
    LOG_DEBUG("Cross product function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        glm::vec3 glmA(random.nextFloat(), random.nextFloat(), random.nextFloat());
        glm::vec3 glmB(random.nextFloat(), random.nextFloat(), random.nextFloat());
        Vec3 a(glmA.x, glmA.y, glmA.z);
        Vec3 b(glmB.x, glmB.y, glmB.z);
        Vec3 c = a ^ b;
        glm::vec3 glmC = glm::cross(glmA, glmB);
        if (!isEqual(c.getX(), glmC[0]))
        {
            error("Vec3 cross product function", c.getX(), glmC[0]);
            break;
        }
        if (!isEqual(c.getY(), glmC[1]))
        {
            error("Vec3 cross product function", c.getY(), glmC[1]);
            break;
        }
        if (!isEqual(c.getZ(), glmC[2]))
        {
            error("Vec3 cross product function", c.getZ(), glmC[2]);
            break;
        }
    }
    errorCount = 0;
    LOG_DEBUG("Rotate function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        float angle = random.nextFloat();
        glm::vec3 glmA(random.nextFloat(), random.nextFloat(), random.nextFloat());
        glm::vec3 glmB(random.nextFloat(), random.nextFloat(), random.nextFloat());
        Vec3 a(glmA.x, glmA.y, glmA.z);
        Vec3 b(glmB.x, glmB.y, glmB.z);
        Vec3 c = a.rotate(b, toRadians(angle));

        glm::mat4 glmRotMatrix = glm::rotate(glm::radians(angle), glmB);
        glm::vec4 glmVec4a = glmRotMatrix * glm::vec4(glmA, 0.0f);

        // std::cout.precision(std::numeric_limits< float >::max_digits10);
        if (!isEqual(c.getX(), glmVec4a[0]))
        {
            error("Vec3 rotate function", c.getX(), glmVec4a[0]);
            break;
        }
        if (!isEqual(c.getY(), glmVec4a[1]))
        {
            error("Vec3 rotate function", c.getY(), glmVec4a[1]);
            break;
        }
        if (!isEqual(c.getZ(), glmVec4a[2]))
        {
            error("Vec3 rotate function", c.getZ(), glmVec4a[2]);
            break;
        }
    }
    errorCount = 0;
    LOG_DEBUG("RotateSelf function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        float angle = random.nextFloat();
        glm::vec3 glmA(random.nextFloat(), random.nextFloat(), random.nextFloat());
        glm::vec3 glmB(random.nextFloat(), random.nextFloat(), random.nextFloat());
        Vec3 a(glmA.x, glmA.y, glmA.z);
        Vec3 b(glmB.x, glmB.y, glmB.z);
        glm::mat4 glmRotMatrix = glm::rotate(glm::radians(angle), glmB);
        Vec3 a2 = a;
        a2.rotateSelf(b, toRadians(angle));

        glm::vec4 glmVec4a2 = glmRotMatrix * glm::vec4(glmA, 0.0f);

        // std::cout.precision(std::numeric_limits< float >::max_digits10);
        if (!isEqual(a2.getX(), glmVec4a2[0]))
        {
            error("Vec3 RotateSelf function", a2.getX(), glmVec4a2[0]);
            break;
        }
        if (!isEqual(a2.getY(), glmVec4a2[1]))
        {
            error("Vec3 RotateSelf function", a2.getY(), glmVec4a2[1]);
            break;
        }
        if (!isEqual(a2.getZ(), glmVec4a2[2]))
        {
            error("Vec3 RotateSelf function", a2.getZ(), glmVec4a2[2]);
            break;
        }
    }
}
void testMat3(const int iterations)
{
    errorCount = 0;
    Random random;
    LOG_DEBUG("****Mat3 testing started****");
    LOG_DEBUG("Inverse function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        Mat3 a(std::array<float, 9>{random.nextFloat(), random.nextFloat(), random.nextFloat(),
                                    random.nextFloat(), random.nextFloat(), random.nextFloat(),
                                    random.nextFloat(), random.nextFloat(), random.nextFloat()});
        glm::mat3 glmMat3;
        glmMat3 = glm::make_mat3(a.getDataPtr());
        glmMat3 = glm::inverse(glmMat3);
        Mat3 novaMat = a.inverse();
        const float *glmMatPtr, *novaMatPtr;
        glmMatPtr = glm::value_ptr(glmMat3);
        novaMatPtr = novaMat.getDataPtr();
        for (int i = 0; i < 9; ++i)
        {
            if (!isEqual(novaMatPtr[i], glmMatPtr[i]))
            {
                error("Mat3 inverse function", novaMatPtr[i], glmMatPtr[i]);
                goto mat3rot;
            }
        }
    }
mat3rot:;
    errorCount = 0;
    LOG_DEBUG("Rotation function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        float angle = random.nextFloat();
        Vec3 v1(random.nextFloat(), random.nextFloat(), random.nextFloat());
        glm::vec3 glmA(v1.getX(), v1.getY(), v1.getZ());
        glm::mat4 glmMat4 = glm::rotate(glm::radians(angle), glmA);
        glm::mat3 glmMat_3(glmMat4);
        const float *glmMatPtr, *novaMatPtr;
        glmMatPtr = glm::value_ptr(glmMat_3);
        Mat3 novaMat = Mat3::makeRotationMatrix(v1, toRadians(angle));
        novaMatPtr = novaMat.getDataPtr();
        for (int i = 0; i < 9; ++i)
        {
            if (!isEqual(novaMatPtr[i], glmMatPtr[i]))
            {
                error("Mat3 rotation function", novaMatPtr[i], glmMatPtr[i]);
                goto mat3end;
            }
        }
    }
mat3end:;
}
void testMat4(const int iterations)
{
    errorCount = 0;
    Random random;
    LOG_DEBUG("****Mat4 testing started****");
    LOG_DEBUG("Identity constructor");
    {
        glm::mat4 temp1(1);
        const float *glmMatPtr = glm::value_ptr(temp1);
        const Mat4 &temp2 = Mat4::makeIdentityMatrix();
        const float *novaMatPtr = temp2.getDataPtr();
        for (int i = 0; i < 16; ++i)
        {
            if (novaMatPtr[i] != glmMatPtr[i])
            {
                error("Mat4 identity constructor", novaMatPtr[i], glmMatPtr[i]);
                goto mat4pers;
            }
        }
    }
mat4pers:;
    errorCount = 0;
    LOG_DEBUG("Mat4 perspective function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        float fov = random.nextFloat(0.000001f, 1000.0f);
        float aspectRatio =
            random.nextFloat(0.000001f, 100000.0f) / random.nextFloat(0.000001f, 100000.0f);
        float nearPlane = random.nextFloat(0.000001f, 100000.0f);
        float farPlane = random.nextFloat(nearPlane, 100000.1f);
        glm::mat4 glmMat = glm::perspectiveRH(glm::radians(fov), aspectRatio, nearPlane, farPlane);
        const float *glmMatPtr = glm::value_ptr(glmMat);
        Mat4 novaMat =
            Mat4::makePerspectiveMatrix(toRadians(fov), aspectRatio, nearPlane, farPlane);
        const float *novaMatPtr = novaMat.getDataPtr();
        for (int i = 0; i < 16; ++i)
        {
            if (!isEqual(novaMatPtr[i], glmMatPtr[i]))
            {
                error("Mat4 perspective function", novaMatPtr[i], glmMatPtr[i]);
                goto mat4rot;
            }
        }
    }
mat4rot:;
    errorCount = 0;
    LOG_DEBUG("Mat4 rotation function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        float angle = random.nextFloat();
        Vec3 v1(random.nextFloat(), random.nextFloat(), random.nextFloat());
        glm::vec3 glmA(v1.getX(), v1.getY(), v1.getZ());
        glm::mat4 glmMat = glm::rotate(glm::radians(angle), glmA);
        const float *glmMatPtr = glm::value_ptr(glmMat);
        Mat4 novaMat = Mat4::makeRotationMatrix(v1, toRadians(angle));
        const float *novaMatPtr = novaMat.getDataPtr();
        for (int i = 0; i < 16; ++i)
        {
            if (!isEqual(novaMatPtr[i], glmMatPtr[i]))
            {
                error("Mat4 rotation function", novaMatPtr[i], glmMatPtr[i]);
                goto mat4mult;
            }
        }
    }
mat4mult:;
    errorCount = 0;
    LOG_DEBUG("Mat4 multiplication function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        Mat4 a(std::array<float, 16>{
            random.nextFloat(), random.nextFloat(), random.nextFloat(), random.nextFloat(),
            random.nextFloat(), random.nextFloat(), random.nextFloat(), random.nextFloat(),
            random.nextFloat(), random.nextFloat(), random.nextFloat(), random.nextFloat(),
            random.nextFloat(), random.nextFloat(), random.nextFloat(), random.nextFloat()});
        Mat4 b(std::array<float, 16>{
            random.nextFloat(), random.nextFloat(), random.nextFloat(), random.nextFloat(),
            random.nextFloat(), random.nextFloat(), random.nextFloat(), random.nextFloat(),
            random.nextFloat(), random.nextFloat(), random.nextFloat(), random.nextFloat(),
            random.nextFloat(), random.nextFloat(), random.nextFloat(), random.nextFloat()});
        glm::mat4 glmMat = glm::make_mat4(a.getDataPtr());
        glm::mat4 glmMat2 = glm::make_mat4(b.getDataPtr());
        glm::mat4 glmResultMat = glmMat2 * glmMat;
        const float *glmMatPtr = glm::value_ptr(glmResultMat);
        const float *novaMatPtr = (a * b).getDataPtr();
        for (int i = 0; i < 16; ++i)
        {
            if (!isEqual(novaMatPtr[i], glmMatPtr[i]))
            {
                error("Mat4 multiplication function", novaMatPtr[i], glmMatPtr[i]);
                goto mat4lookat;
            }
        }
    }
mat4lookat:;
    errorCount = 0;
    LOG_DEBUG("Mat4 lookAt function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        glm::vec3 glmA(random.nextFloat(), random.nextFloat(), random.nextFloat());
        glm::vec3 glmB(random.nextFloat(), random.nextFloat(), random.nextFloat());
        glm::vec3 glmC(random.nextFloat(), random.nextFloat(), random.nextFloat());
        Vec3 v1(glmA.x, glmA.y, glmA.z);
        Vec3 v2(glmB.x, glmB.y, glmB.z);
        Vec3 v3(glmC.x, glmC.y, glmC.z);

        glm::mat4 glmMat = glm::lookAtLH(glmA, glmB, glmC);
        const float *glmMatPtr = glm::value_ptr(glmMat);

        Mat4 novaMat = Mat4::makeLookAtMatrix(v1, v2, v3);
        const float *novaMatPtr = novaMat.getDataPtr();

        for (int i = 0; i < 16; ++i)
        {
            if (!isEqual(novaMatPtr[i], glmMatPtr[i]))
            {
                error("Mat4 lookAt function", novaMatPtr[i], glmMatPtr[i]);
                novaMat.debugPrint();

                double dArray[16] = {0.0};
                const float *pSource = (const float *)glm::value_ptr(glmMat);
                for (int i = 0; i < 16; ++i)
                {
                    dArray[i] = pSource[i];
                    std::cout << dArray[i] << " ";
                }

                goto mat4end;
            }
        }
    }
mat4end:;
}
void testQuat(const int iterations)
{
    errorCount = 0;
    Random random;
    LOG_DEBUG("****Quaternion testing started****");
    LOG_DEBUG("Quaternion from Euler angles constructor");
    for (int iter = 0; iter < iterations; ++iter)
    {
        float a = random.nextFloat(-10000.0f, 10000.0f), b = random.nextFloat(-10000.0f, 10000.0f),
              c = random.nextFloat(-10000.0f, 10000.0f);
        UnitQuat novaEuler(toRadians(a), toRadians(b), toRadians(c));
        glm::quat glmEuler = glm::quat(glm::vec3(toRadians(a), toRadians(b), toRadians(c)));
        LOG_DEBUG(a << " " << b << " "
                    << " " << c);
        LOG_DEBUG(novaEuler.getX() << " " << glmEuler.x);
        LOG_DEBUG(novaEuler.getY() << " " << glmEuler.y);
        LOG_DEBUG(novaEuler.getZ() << " " << glmEuler.z);
        LOG_DEBUG(novaEuler.getW() << " " << glmEuler.w);
        LOG_DEBUG(" ");
        if (!isEqual(novaEuler.getX(), glmEuler.x))
        {
            error("Quaternion from Euler angles constructor ", novaEuler.getX(), glmEuler.x);
            break;
        }
        if (!isEqual(novaEuler.getY(), glmEuler.y))
        {
            error("Quaternion from Euler angles constructor ", novaEuler.getY(), glmEuler.y);
            break;
        }
        if (!isEqual(novaEuler.getZ(), glmEuler.z))
        {
            error("Quaternion from Euler angles constructor ", novaEuler.getZ(), glmEuler.z);
            break;
        }
        if (!isEqual(novaEuler.getW(), glmEuler.w))
        {
            error("Quaternion from Euler angles constructor ", novaEuler.getW(), glmEuler.w);
            break;
        }
    }
    errorCount = 0;
    LOG_DEBUG("Quaternion multiplication function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        UnitQuat a(random.nextFloat(), random.nextFloat(), random.nextFloat(), random.nextFloat());
        UnitQuat b(random.nextFloat(), random.nextFloat(), random.nextFloat(), random.nextFloat());
        UnitQuat c = a * b;
        glm::quat quat1(a.getW(), glm::vec3(a.getX(), a.getY(), a.getZ()));
        glm::quat quat2(b.getW(), glm::vec3(b.getX(), b.getY(), b.getZ()));
        glm::quat quat3 = quat1 * quat2;
        if (!isEqual(c.getX(), quat3.x))
        {
            error("Quaternion multiplication function", c.getX(), quat3.x);
            break;
        }
        if (!isEqual(c.getY(), quat3.y))
        {
            error("Quaternion multiplication function", c.getY(), quat3.y);
            break;
        }
        if (!isEqual(c.getZ(), quat3.z))
        {
            error("Quaternion multiplication function", c.getZ(), quat3.z);
            break;
        }
        if (!isEqual(c.getW(), quat3.w))
        {
            error("Quaternion multiplication function", c.getW(), quat3.w);
            break;
        }
    }
    // errorCount = 0;
    // LOG_DEBUG("Quaternion rotation function");
    // for (int iter = 0; iter < iterations; ++iter)
    // {
    // 	float angle = random.nextFloat();
    // 	float a = random.nextFloat(), b =
    // random.nextFloat(), c =
    random.nextFloat();
    // 	float d = random.nextFloat(), e =
    // random.nextFloat(), f =
    random.nextFloat();
    // 	Vec3 v(a,b,c);
    // 	UnitQuat quat(Vec3(d, e, f), toRadians(angle));
    // 	v = v.rotate(quat);
    // }
    errorCount = 0;
    LOG_DEBUG("Quaternion toRotationMatrix4 function");
    for (int iter = 0; iter < iterations; ++iter)
    {
        float angle = random.nextFloat();
        Vec3 v1(random.nextFloat(), random.nextFloat(), random.nextFloat());
        glm::vec3 glmA(v1.getX(), v1.getY(), v1.getZ());
        glm::mat4 glmMat = glm::rotate(glm::radians(angle), glmA);
        const float *glmMatPtr = glm::value_ptr(glmMat);
        Mat4 MatFromQuat = UnitQuat(v1, toRadians(angle)).toRotationMatrix4();
        Mat4 rotMat = Mat4::makeRotationMatrix(v1, toRadians(angle));
        for (int i = 0; i < 16; ++i)
        {
            if (!isEqual(rotMat.getDataPtr()[i], glmMatPtr[i]))
            {
                error("Quaternion toRotationMatrix4 function", rotMat.getDataPtr()[i],
                      glmMatPtr[i]);
                goto quatend;
            }
            LOG_DEBUG(MatFromQuat.getDataPtr()[i] << "  " << rotMat.getDataPtr()[i] << "  "
                                                  << glmMatPtr[i]);
        }
    }
quatend:;
}

void testMatLibrary()
{
    LOG_DEBUG("Running Tests");
    int iterations = 10;
    LOG_DEBUG("Testing " << iterations << " iterations");
    testVec3(iterations);
    testMat3(iterations);
    testMat4(iterations);
    testQuat(iterations);
    LOG_DEBUG("Tests Finished");
    if (errorCountTotal == 0)
    {
        LOG_DEBUG("All test passed");
    }
    else
    {
        LOG_DEBUG(errorCountTotal << " errors found");
    }
}
void error(const std::string &failedFunctionName, const float calculatedValue,
           const float correctValue)
{
    if (errorCount < 1)
    { // only log the first error
        LOG_WARNING("TEST FAILED! in " << failedFunctionName);
        LOG_WARNING("got " << calculatedValue << " should be " << correctValue);
    }
    errorCount++;
    errorCountTotal++;
}
} // namespace tests
} // namespace Nova