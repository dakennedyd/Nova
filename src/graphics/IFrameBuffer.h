#pragma once
#include "Bindable.h"
#include "resource_manager/ResourceBase.h"

namespace Nova
{
class IFrameBuffer : public Bindable, public ResourceBase
{
  public:
    IFrameBuffer() = default;
    virtual ~IFrameBuffer() = default;
};

} // namespace Nova