#pragma once
#include "graphics/Bindable.h"
#include "resource_manager/ResourceBase.h"

namespace Nova
{
enum MipMapping
{
    NO_MIPMAP,
    GENERATE_MIPMAP
};
enum Filtering
{
    LINEAR,
    NEAREST
};
enum TextureType
{
    COLOR,
    COLOR_ALPHA,
    DEPTHBUFFER,
    DEPTHSTENCIL,
    GBUFFER_NORMAL,
    GBUFFER_POSITION,
    MONOCHROME,
    DUAL_COLOR,
    NORMAL_MAP,
    COLOR_HDR
};

class ITexture : public Bindable, public ResourceBase
{
  public:
    virtual ~ITexture() = default;
};
} // namespace Nova
