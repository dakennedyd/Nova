#pragma once
#include "graphics/Bindable.h"
#include "graphics/IGPUProgramParameter.h"

namespace Nova
{
class IRenderPacket : public Bindable
{
  public:
    IRenderPacket() = default;
    virtual ~IRenderPacket() = default;

    virtual void addParameter(IGPUProgramParameter *parameter) = 0;
    virtual void updateAllUniforms() const = 0;
    virtual void draw() const = 0;
};
} // namespace Nova
