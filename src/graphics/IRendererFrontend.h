#pragma once

namespace Nova
{
class IRendererFrontend
{
  public:
    IRendererFrontend() = default;
    virtual ~IRendererFrontend() = default;
    virtual void createRenderPackets() = 0;
};
} // namespace Nova