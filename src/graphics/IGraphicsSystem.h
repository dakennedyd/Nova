#pragma once

namespace Nova
{
class RenderPacket;

class IGraphicsSystem
{
  public:
    IGraphicsSystem() = default;
    virtual ~IGraphicsSystem() = default;
    // virtual void render() = 0;
    // virtual void addPacket(RenderPacket *primitive) = 0;
    // virtual Camera* getCurrentCamera() = 0;				//needed by opengl
    // virtual void setCurrentCamera(Camera* newCamera) = 0;
  protected:
};
} // namespace Nova
