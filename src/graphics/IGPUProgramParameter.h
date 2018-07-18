#pragma once

namespace Nova
{
class IGPUProgramParameter
{
  public:
    virtual ~IGPUProgramParameter() = default;
    virtual void update() = 0;
};
} // namespace Nova