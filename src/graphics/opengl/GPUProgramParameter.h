#pragma once
#include "graphics/IGPUProgramParameter.h"
#include <glad/glad.h>

namespace Nova
{
class GPUProgramParameterMat4 : public IGPUProgramParameter
{
  public:
    GPUProgramParameterMat4() = delete;
    GPUProgramParameterMat4(const GLuint location, const float *parameter);
    ~GPUProgramParameterMat4() = default;

    void update() override;

  private:
    const float *mData;
    const GLuint mLocation;
};

class GPUProgramParameterVec3 : public IGPUProgramParameter
{
  public:
    GPUProgramParameterVec3() = delete;
    GPUProgramParameterVec3(const GLuint location, const float *parameter);
    // GPUProgramParameterVec3::GPUProgramParameterVec3(const GLuint location, const float x, const
    // float y, const float z);
    ~GPUProgramParameterVec3() = default;

    void update() override;

  private:
    const float *mData;
    // const float mData[3];
    const GLuint mLocation;
};

class GPUProgramParameterVec4 : public IGPUProgramParameter
{
  public:
    GPUProgramParameterVec4() = delete;
    GPUProgramParameterVec4(const GLuint location, const float *parameter);
    ~GPUProgramParameterVec4() = default;

    void update() override;

  private:
    const float *mData;
    const GLuint mLocation;
};

class GPUProgramParameterFloat : public IGPUProgramParameter
{
  public:
    GPUProgramParameterFloat() = delete;
    GPUProgramParameterFloat(const GLuint location, const float parameter);
    ~GPUProgramParameterFloat() = default;

    void update() override;

  private:
    const float mData;
    const GLuint mLocation;
};

class GPUProgramParameterInt : public IGPUProgramParameter
{
  public:
    GPUProgramParameterInt() = delete;
    GPUProgramParameterInt(const GLuint location, const int parameter);
    ~GPUProgramParameterInt() = default;

    void update() override;

  private:
    const int mData;
    const GLuint mLocation;
};
} // namespace Nova
