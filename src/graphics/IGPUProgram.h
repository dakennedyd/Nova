#pragma once
//#include "graphics/IGPUProgramParameter.h"
#include "graphics/Bindable.h"
#include "resource_manager/ResourceBase.h"

namespace Nova
{
class IGPUProgram : public Bindable, public ResourceBase
{
  public:
    virtual ~IGPUProgram() = default;

    // virtual void addParameter(IGPUProgramParameter* parameter) = 0;
    // virtual void updateAllUniforms() = 0;
};
} // namespace Nova
